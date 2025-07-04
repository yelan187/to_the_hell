#include "Model/Entities/Animation.h"
#include <iostream>

using namespace Model::Entities;

Animation::Animation(int id, const std::string& texture_path, sf::Vector2f position, 
                    sf::Vector2f size, int frame_cols, int frame_rows, 
                    float animation_duration, bool loop)
    : id(id), texture_path(texture_path), position(position), size(size),
      frame_cols(frame_cols), frame_rows(frame_rows), 
      animation_duration(animation_duration), loop(loop),
      current_frame(0), current_time(0.0f), finished(false) {
    
    total_frames = frame_cols * frame_rows;
    time_per_frame = animation_duration / total_frames;
    
    // 计算每一帧在spritesheet中的大小
    // 假设spritesheet的总大小需要根据实际图片确定，这里先用默认值
    // 实际使用时可能需要从纹理获取真实大小
    frame_size = sf::Vector2f(size.x / frame_cols, size.y / frame_rows);
}

Animation::~Animation() {
}

void Animation::update(float delta_time) {
    if (finished && !loop) {
        return;
    }
    
    current_time += delta_time;
    
    if (current_time >= time_per_frame) {
        current_frame++;
        current_time = 0.0f;
        
        if (current_frame >= total_frames) {
            if (loop) {
                current_frame = 0;
            } else {
                current_frame = total_frames - 1;
                finished = true;
            }
        }
    }
}

sf::IntRect Animation::getCurrentFrameRect() const {
    int row = current_frame / frame_cols;
    int col = current_frame % frame_cols;
    
    // Hit-Yellow.png 是4x4的spritesheet，总大小为4096x4096
    // 每帧的大小为 4096/4 = 1024x1024
    int frame_pixel_width = 1024;  // 每帧的像素宽度
    int frame_pixel_height = 1024; // 每帧的像素高度
    
    return sf::IntRect(
        col * frame_pixel_width,
        row * frame_pixel_height,
        frame_pixel_width,
        frame_pixel_height
    );
}
