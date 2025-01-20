mkdir 5final
cd 5final
gcc ../main.c
./a.out
ffmpeg -i %d.png -pix_fmt yuv420p output.mp4
rm *.png