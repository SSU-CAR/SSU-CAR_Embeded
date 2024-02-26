from setuptools import setup

package_name = 'phone_detection'

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
    description='phone detection',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'phone_detector_pub = phone_detection.phone_detector:main'
        ],
    },
)
