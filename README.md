[![standard-readme compliant](https://img.shields.io/badge/readme%20style-standard-brightgreen.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)

# Ray Tracer
This basic ray tracer program renders the scene described in text files, and generate images by tracing the path of light as pixels in an image plane and simulating the effects of object intersections, shading, shadows, and reflections.
<p align="center">
<img src="tests/solutions/25.png" width="150"/> <img src="tests/solutions/27.png" width="150"/>
</p>

## Usage
This project is tested on MacOS Catalina 10.15.6 and Ubuntu 16.04 LTS. Make sure you have [SCons](https://scons.org/pages/download.html).
```sh
$ git clone https://github.com/jia7xiong/ray-tracer.git
$ cd ray-tracer
```

### Run
 ```sh
$ scons
$ ./ray_tracer -i <description-file>
 ```
The result is dumped to [output.png](output).
### Test
1. Render the [test-file](tests/cases), dump the result to [output.png](output), then compare the result to the [solution-file](tests/solutions) and dump the error to [diff.png](output).
```
$ ./ray_tracer -i <test-file> [-s <solution-file>]
```
2. Print a measure of the error for each of the [test files](tests/cases). Adapt [testing-scheme.txt](tests/testing-scheme.txt) for your own test cases if needed.
```sh
$ ./testing-script.py
```

### Get rid of object and executable files
```sh
$ scons -c
```