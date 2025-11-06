from glob import glob
import os
from setuptools import setup

package_name = 'saye_mppi'

launch_files = glob(os.path.join('launch', '*.launch.py'))
config_files = glob(os.path.join('config', '*.yaml'))
world_files = glob(os.path.join('worlds', '*.sdf'))

data_files = [
    ('share/ament_index/resource_index/packages',
     ['resource/' + package_name]),
    ('share/' + package_name, ['package.xml']),
]

if launch_files:
    data_files.append(
        (os.path.join('share', package_name, 'launch'), launch_files))
if config_files:
    data_files.append(
        (os.path.join('share', package_name, 'config'), config_files))
if world_files:
    data_files.append(
        (os.path.join('share', package_name, 'worlds'), world_files))

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=data_files,
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='SAYE Maintainers',
    maintainer_email='alitekes123@gmail.com',
    description='Utilities for MPPI-focused experiments in the SAYE simulation.',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'straight_path_commander = saye_mppi.path_commander:main',
            'moving_obstacles = saye_mppi.moving_obstacles:main',
            'pedestrian_manager = saye_mppi.pedestrian_manager:main',
        ],
    },
)
