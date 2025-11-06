from setuptools import find_packages
from setuptools import setup

setup(
    name='saye_mppi',
    version='0.0.0',
    packages=find_packages(
        include=('saye_mppi', 'saye_mppi.*')),
)
