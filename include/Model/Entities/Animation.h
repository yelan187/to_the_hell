#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace Model {
namespace Entities {

/**
 * Animation类 - 处理sprite动画效果
 * 支持帧动画播放，可以设置帧数、播放时间、循环等属性
 */
class Animation {
public:
    Animation(int id, const std::string& texture_path, sf::Vector2f position, 
              sf::Vector2f size, int frame_cols, int frame_rows, 
              float animation_duration, bool loop = false);
    ~Animation();

    void update(float delta_time);
    bool isFinished() const { return finished; }
    
    // Getters
    int getId() const { return id; }
    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getSize() const { return size; }
    int getCurrentFrame() const { return current_frame; }
    int getTotalFrames() const { return total_frames; }
    std::string getTexturePath() const { return texture_path; }
    sf::Vector2f getFrameSize() const { return frame_size; }
    
    // 获取当前帧在spritesheet中的位置
    sf::IntRect getCurrentFrameRect() const;

private:
    int id;
    sf::Vector2f position;
    sf::Vector2f size;
    std::string texture_path;
    
    int frame_cols;
    int frame_rows;
    int total_frames;
    int current_frame;
    sf::Vector2f frame_size;
    
    float animation_duration;
    float time_per_frame;
    float current_time;
    bool loop;
    bool finished;
};

}
}
