The script `main.c` is a simple rasterizer in c, i.e. it draws on screen geometric objects. It's intended to use with ffmpeg to create videos, using the following commands (also in `create_video.sh`):

```bash
mkdir tmp_dirx
cd tmp_dirx
gcc ../main.c
./a.out
ffmpeg -i %d.png -pix_fmt yuv420p output.mp4
rm *.png
mv output.mp4 ../videos/output.mp4
cd ..
rm -rf tmp_dirx
```

I originally started this project to create this:

<video controls src="videos/disappearing_effect.mp4" title="Title"></video>


https://github.com/user-attachments/assets/2523892e-1be7-4384-81cb-c5ba89663b17



A video of a rotating cube with the peculiarity that the cube disappears when pausing the video!
