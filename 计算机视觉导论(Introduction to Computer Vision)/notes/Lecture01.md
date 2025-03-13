# Lecture 01. Image, Filter, and Edge Detection

## Image

### Image as Functions

A 2-D image can be treated as a function $f: \mathbb{R}^2\rightarrow \mathbb{R}^M$.

$f(x,y)$: the *intensity* at position $(x, y)$.

$M$ is # of channels. E.g., for a colored RGB image: $f(x,y) = [r(x,y), g(x,y), b(x,y)]^\text{T} \in [0,255]^3$.

### Analog2Digital

An (digital) image contains discrete number of pixels. So here the image function $f$ is from $\mathbb{N}^2$ to $\mathbb{R}^M$.

### Image Gradient
$$\nabla f=\left[\frac{\partial f}{\partial x},\frac{\partial f}{\partial y}\right]$$

Gradient magnitude:

$$||\nabla f||=\sqrt{\left(\frac{\partial f}{\partial x}\right)^2+\left(\frac{\partial f}{\partial y}\right)^2}$$

In practice, use finite difference to replace (discretize) gradient.

$$\left.\frac{\partial f}{\partial x}\right|_{x=x_0}\approx\frac{f(x_0+1, y_0)-f(x_0-1,y_0)}{2}$$

The image gradient points in the direction of **the most rapid change** in *intensity*.

## Filters

### 1-D Filter: Moving Average

Signal function: $f[n]$. Signal processing system: $g$. Processed signal function: $h[n]$.

Thus, $h=g(f), h[n]=g(f)[n]$.

For a linear system, the signal processing process can be described by **convolution**.  

#### 1. Linear System

#### 2. Definition of 1-D Convolution ($*$) ####

On descrete signal:  
$$h[n]=(f*g)[n]=\sum_{m=-\infty}^\infty f[m]g[n-m]$$

On continuous signal:

$$h(x)=(f*g)(x)=\int_{m=-\infty}^\infty f(m)g(x-m)\text{d}m$$

> Note：  
> 1.Convolution operates on functions.  
> 2.How to understand convolution? The function $g$ is flipped horizontally around $n$, then it is multiplied element-wise with function $f$ and summed.

#### 3. Convolution and Fourier Transform

