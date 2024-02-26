from setuptools import setup

package_name = 'cam2node'

setup(
    name=package_name,
    version='0.0.1',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='byungjikim',
    maintainer_email='qudwl33@naver.com',
    description='Ros2 image node from /dev/video0',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'cam_pub = usbcam2node.usbcam_pub:main'
        ],
    },
)
