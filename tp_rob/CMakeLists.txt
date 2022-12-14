cmake_minimum_required(VERSION 3.0.2)
project(tp_rob)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  message_generation
)

add_message_files(
  FILES
  Motor.msg
)

add_service_files(
  FILES
  GetMotorData.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

# TP1
add_executable(subscriber src/subscriber.cpp)
target_link_libraries(subscriber ${catkin_LIBRARIES})

add_executable(publisher src/publisher.cpp)
target_link_libraries(publisher ${catkin_LIBRARIES})

# TP2
add_executable(publisher_motor src/publisher_motor.cpp)
target_link_libraries(publisher_motor ${catkin_LIBRARIES})
add_dependencies(publisher_motor ${PROJECT_NAME}_generate_messages_cpp)

add_executable(get_motor_data_client src/service_client_motor.cpp)
target_link_libraries(get_motor_data_client ${catkin_LIBRARIES})
add_dependencies(get_motor_data_client ${PROJECT_NAME}_generate_messages_cpp)

# TP3
add_executable(one_dof_robot src/one_dof_robot.cpp)
target_link_libraries(one_dof_robot ${catkin_LIBRARIES})
add_dependencies(one_dof_robot ${PROJECT_NAME}_generate_messages_cpp)

# TP4
add_executable(turtlesim_controller src/turtlesim_controller.cpp)
target_link_libraries(turtlesim_controller ${catkin_LIBRARIES})
add_dependencies(turtlesim_controller ${PROJECT_NAME}_generate_messages_cpp)
