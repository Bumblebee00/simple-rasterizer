mkdir strano
cd strano
gcc ../main.c -o stranoo
./stranoo
ffmpeg -i %d.png -pix_fmt yuv420p output.mp4
rm *.png