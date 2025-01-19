mkdir cube_linee
cd cube_linee
gcc ../main.c -o simple_cube_linee
./simple_cube_linee
ffmpeg -i %d.png -pix_fmt yuv420p output.mp4
rm *.png