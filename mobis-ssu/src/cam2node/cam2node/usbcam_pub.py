import rclpy
import cv2
import numpy as np

from rclpy.node import Node
from sensor_msgs.msg import Image
from std_msgs.msg import Header
from cv_bridge import CvBridge

class usbCamNode(Node):
    def __init__(self):
        super().__init__('camera_node')
        self.publisher = self.create_publisher(Image, '/camera/image', 10)
        self.cap = cv2.VideoCapture(0)  # USB 카메라에 연결되어 있는 경우 인덱스 0을 사용
        self.cv_bridge = CvBridge()


    def publish_image(self):
        ret, frame = self.cap.read()
        if not ret:
            self.get_logger().warn('Failed to read image from camera')
            return
        image_msg = self.cv_bridge.cv2_to_imgmsg(frame, encoding='bgr8')
        # 이미지를 ROS 메시지로 변환
        # height, width, channels = frame.shape

        # # ROS 2 이미지 메시지 헤더 설정
        # header = Header()
        # header.frame_id = 'camera_frame'
        # header.stamp = self.get_clock().now().to_msg()

        # # frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        # # 이미지를 ROS 2 이미지 메시지로 변환
        # image_msg = Image()
        # image_msg.header = header
        # image_msg.height = height
        # image_msg.width = width
        # # print(height, width)
        # # image_msg.encoding = 'rgb8'
        # image_msg.encoding = 'bgr8'  
        # # image_msg.data = np.array(frame_rgb).tobytes()
        # image_msg.data = np.array(frame).tobytes()  

        self.publisher.publish(image_msg)

def main(args=None):
    rclpy.init(args=args)
    camera_node = usbCamNode()
    while rclpy.ok():
        camera_node.publish_image()
        
    camera_node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()