The script `main.c` creates images of a hollow cube seen from a orbiting camera, that can be joined in a video using ffmpeg. The peculiarity is that the video starts from a random pixel configuration, and only the pixels in the cube are updated. Therfore a rotating cube is visible, but when pausing the video the cube disappears!
<video controls src="5final/output.mp4" title="Title"></video>

Here are the bash commands to create the video (also in `create_video.sh`):
```bash
mkdir foo
cd foo
gcc ../main.c
./a.out
ffmpeg -i %d.png -pix_fmt yuv420p output.mp4
rm *.png
```
In the repo there are other videos created while writing this project