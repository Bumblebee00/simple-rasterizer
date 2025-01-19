mkdir cube
cd cube
gcc ../main.c -o simple_cube
./simple_cube
ffmpeg -i %d.png -pix_fmt yuv420p output.mp4
rm *.png