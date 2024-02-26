from setuptools import setup

package_name = 'head_pose_estimation'

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
    description='provide estimation your head pose',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'head_pose_pub = head_pose_estimation.head_pose_estimation:main'
        ],
    },
)
