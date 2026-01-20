from numpy.random import randn, randint, rand
from numpy import zeros, ones


def kaiming_uniform(indim, outdim):
    return (3/indim)**0.5 *(2*rand(indim, outdim) - 1) 


def kaiming_normal(indim, outdim):
    return (2/indim)**0.5*randn(indim, outdim)