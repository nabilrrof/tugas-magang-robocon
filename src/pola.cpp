#include <cmath>
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/srv/teleport_absolute.hpp"
#include "turtlesim/srv/set_pen.hpp"

class PolaTurtleSim : public rclcpp::Node {
public:
    PolaTurtleSim() : Node("pola_publisher") {
        pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
        teleport_client_ = this->create_client<turtlesim::srv::TeleportAbsolute>("/turtle1/teleport_absolute");
        set_pen_client_ = this->create_client<turtlesim::srv::SetPen>("/turtle1/set_pen");
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(10),
            std::bind(&PolaTurtleSim::timer_callback, this)
        );
    }

private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;
    rclcpp::Client<turtlesim::srv::TeleportAbsolute>::SharedPtr teleport_client_;
    rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr set_pen_client_;
    rclcpp::TimerBase::SharedPtr timer_;

    int fase_ = 0;
    int step_ = 0;
    bool sedang_bergerak_ = false;
    rclcpp::Time waktu_mulai_ = rclcpp::Time();

    void teleport(float x, float y, float theta) {
        auto request = std::make_shared<turtlesim::srv::TeleportAbsolute::Request>();
        request->x = x;
        request->y = y;
        request->theta = theta;
        teleport_client_->async_send_request(request);
    }

    void set_pen(bool angkat_pena) {
        auto request = std::make_shared<turtlesim::srv::SetPen::Request>();

        request->r = 255;
        request->g = 255;
        request->b = 255;
        request->width = 3;

        request->off = angkat_pena ? 1 : 0;

        set_pen_client_->async_send_request(request);
    }

    void timer_callback() {
        auto sekarang = this->now();

        if(fase_ == 0) {
            set_pen(true);
            teleport(1.0, 1.0, M_PI / 2.0);
            fase_ = 1;
        }
        else if(fase_ == 1) {
            auto msg = geometry_msgs::msg::Twist();

            if(!sedang_bergerak_) {
                waktu_mulai_ = sekarang;
                sedang_bergerak_ = true;
                set_pen(false);
            }

            double durasi_target = M_PI / 3.0;

            if((sekarang - waktu_mulai_).seconds() < durasi_target) {
                msg.linear.x = 4.5;
                msg.angular.z = -3.0;
                pub_->publish(msg);
            } 
            else {
                msg.linear.x = 0.0;
                msg.angular.z = 0.0;
                pub_->publish(msg);

                sedang_bergerak_ = false;
                step_++;

                if(step_ < 12) {
                    float next_x = 1.0, next_y = 1.0, next_theta = 0.0;

                    if (step_ == 1) { next_x = 4.0; next_y = 1.0; next_theta = M_PI / 2.0; }
                    else if (step_ == 2) { next_x = 7.0; next_y = 1.0; next_theta = M_PI / 2.0; }
                    
                    else if (step_ == 3) { next_x = 10.0; next_y = 1.0; next_theta = M_PI; }
                    else if (step_ == 4) { next_x = 10.0; next_y = 4.0; next_theta = M_PI; }
                    else if (step_ == 5) { next_x = 10.0; next_y = 7.0; next_theta = M_PI; }
                    
                    else if (step_ == 6) { next_x = 10.0; next_y = 10.0; next_theta = 3.0 * M_PI / 2.0; }
                    else if (step_ == 7) { next_x = 7.0; next_y = 10.0; next_theta = 3.0 * M_PI / 2.0; }
                    else if (step_ == 8) { next_x = 4.0; next_y = 10.0; next_theta = 3.0 * M_PI / 2.0; }
                    
                    else if (step_ == 9) { next_x = 1.0; next_y = 10.0; next_theta = 0.0; }
                    else if (step_ == 10) { next_x = 1.0; next_y = 7.0; next_theta = 0.0; }
                    else if (step_ == 11) { next_x = 1.0; next_y = 4.0; next_theta = 0.0; }

                    teleport(next_x, next_y, next_theta);
                }
                else {
                    set_pen(true);
                    fase_ = 2;
                    step_ = 0;
                    teleport(4.0, 4.0, 0.0);
                }
            }

        }
        else if(fase_ == 2) {
            auto msg = geometry_msgs::msg::Twist();

            if (!sedang_bergerak_) {
                waktu_mulai_ = sekarang;
                sedang_bergerak_ = true;
                
                if (step_ == 7 || step_ == 8 || step_ == 12 || step_ == 13) {
                    set_pen(true);
                } else {
                    set_pen(false);
                }
            }

            double durasi_target = 2.0 * M_PI / 3.0;

            if((sekarang - waktu_mulai_).seconds() < durasi_target) {
                msg.linear.x = 4.5;
                msg.angular.z = -3.0;
                pub_->publish(msg);
            }
            else {
                msg.linear.x = 0.0;
                msg.angular.z = 0.0;
                pub_->publish(msg);

                sedang_bergerak_ = false;
                step_++;

                if(step_ < 16) {
                    float next_x = 4.0, next_y = 4.0, next_theta = 0.0;

                    if(step_ / 4 == 0) {next_x = 4.0; next_y = 4.0;}
                    else if(step_ / 4 == 1) {next_x = 7.0; next_y = 4.0;}
                    else if(step_ / 4 == 2) {next_x = 7.0; next_y = 7.0;}
                    else if(step_ / 4 == 3) {next_x = 4.0; next_y = 7.0;}

                    if(step_ % 4 == 0) {next_theta = 0.0; set_pen(true);}
                    else if(step_ % 4 == 1) {next_theta = M_PI / 2.0;}
                    else if(step_ % 4 == 2) {next_theta = M_PI;}
                    else if(step_ % 4 == 3) {next_theta = 3.0 * M_PI / 2.0;}

                    teleport(next_x, next_y, next_theta);
                }
                else {
                    fase_ = 3;
                }
            }
        }
        else if(fase_ == 3) {
            set_pen(true);
            teleport(5.5, 5.5, M_PI / 2);

            RCLCPP_INFO(this->get_logger(), "[END] Pola telah selesai digambar.");

            fase_ = 4;
        }
    }

    
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PolaTurtleSim>());
    rclcpp::shutdown();

    return 0;
}