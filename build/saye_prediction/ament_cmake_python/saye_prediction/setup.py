from setuptools import find_packages
from setuptools import setup

setup(
    name='saye_prediction',
    version='0.0.1',
    packages=find_packages(
        include=('saye_prediction', 'saye_prediction.*')),
)
