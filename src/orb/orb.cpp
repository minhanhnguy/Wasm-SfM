#ifndef ORB_H
#define ORB_H
#include "../stb/stb_image.h"
#include "../stb/stb_image_write.h"
#include <cmath>
#include <vector>
#include <algorithm>

static int compute_corner_score(unsigned char* image, int width, int height, int x, int y, int threshold) {
    const int neighbors[16][2] = {
        {3, 0}, {3, 1}, {2, 2}, {1, 3}, 
        {0, 3}, {-1, 3}, {-2, 2}, {-3, 1}, 
        {-3, 0}, {-3, -1}, {-2, -2}, {-1, -3}, 
        {0, -3}, {1, -3}, {2, -2}, {3, -1}
    };
    
    int center = image[y * width + x];
    int score = 0;
    
    for (int i = 0; i < 16; ++i) {
        int nx = x + neighbors[i][0];
        int ny = y + neighbors[i][1];
        int neighbor = image[ny * width + nx];
        int diff = std::abs(neighbor - center);
        score += diff;
    }
    
    return score;
}

static std::vector<std::vector<int>> detect_keypoints_with_threshold(unsigned char* image, int width, int height, int threshold) {    
    std::vector<std::vector<int>> keypoint_candidates;
    
    const int neighbors[16][2] = {
        {3, 0}, {3, 1}, {2, 2}, {1, 3}, 
        {0, 3}, {-1, 3}, {-2, 2}, {-3, 1}, 
        {-3, 0}, {-3, -1}, {-2, -2}, {-1, -3}, 
        {0, -3}, {1, -3}, {2, -2}, {3, -1}
    };

    for (int y = 3; y < height - 3; ++y) {
        for (int x = 3; x < width - 3; ++x) {
            int center = image[y * width + x];

            int brighter_count = 0;
            int darker_count = 0;
            const int initial_check[4] = {0, 4, 8, 12}; 
            
            for (int i = 0; i < 4; ++i) {
                int nx = x + neighbors[initial_check[i]][0];
                int ny = y + neighbors[initial_check[i]][1];
                int neighbor = image[ny * width + nx];
                if (neighbor > center + threshold) {
                    brighter_count++;
                } else if (neighbor < center - threshold) {
                    darker_count++;
                }
            }

            if (brighter_count >= 2 || darker_count >= 2) { 
                const int N = 12;
                for (int start = 0; start < 16; ++start) {
                    bool all_brighter = true;
                    bool all_darker = true;

                    for (int i = 0; i < N; ++i) {
                        int idx = (start + i) % 16;
                        int nx = x + neighbors[idx][0];
                        int ny = y + neighbors[idx][1];
                        int neighbor = image[ny * width + nx];

                        if (neighbor <= center + threshold) {
                            all_brighter = false;
                        }
                        if (neighbor >= center - threshold) {
                            all_darker = false;
                        }
                        if (!all_brighter && !all_darker) {
                            break;
                        }
                    }

                    if (all_brighter || all_darker) {
                        int score = compute_corner_score(image, width, height, x, y, threshold);
                        keypoint_candidates.push_back({x, y, score});
                        break; 
                    }
                }
            }
        }
    }

std::vector<std::vector<int>> keypoints;
    for (const auto& candidate : keypoint_candidates) {
        int x = candidate[0];
        int y = candidate[1];
        int score = candidate[2];
        bool is_max = true;
        
        for (int dy = -1; dy <= 1 && is_max; ++dy) {
            for (int dx = -1; dx <= 1 && is_max; ++dx) {
                if (dx == 0 && dy == 0) continue;
                
                for (const auto& neighbor : keypoint_candidates) {
                    if (neighbor[0] == x + dx && neighbor[1] == y + dy) {
                        if (neighbor[2] > score) {
                            is_max = false;
                            break;
                        }
                    }
                }
            }
        }
        
        if (is_max) {
            keypoints.push_back({x, y});
        }
    }

    if (keypoint_candidates.size() > 2000) {
        std::sort(keypoint_candidates.begin(), keypoint_candidates.end(), 
                 [](const std::vector<int>& a, const std::vector<int>& b) {
                     return a[2] > b[2];
                 });
        
        keypoints.clear();
        for (int i = 0; i < 2000 && i < keypoint_candidates.size(); ++i) {
            keypoints.push_back({keypoint_candidates[i][0], keypoint_candidates[i][1]});
        }
    }

    return keypoints;
}

std::vector<std::vector<int>> detect_keypoints(unsigned char* image, int width, int height) {
    int min_keypoints = 2000; 
    int max_keypoints = 3000; 
    int low = 5;         
    int high = 100;       
    int threshold = 0;
    std::vector<std::vector<int>> keypoints;

    for (int iter = 0; iter < 10; ++iter) { 
        threshold = int((low + high) / 2);
        keypoints = detect_keypoints_with_threshold(image, width, height, threshold); 
        int num_keypoints = keypoints.size();

        if (num_keypoints >= min_keypoints && num_keypoints <= max_keypoints) {
            break;
        } else if (num_keypoints < min_keypoints) {
            high = threshold;
        } else {
            low = threshold;
        }
    }

    return keypoints;
}

#endif