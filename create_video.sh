mkdir tmp_dirx
cd tmp_dirx
gcc ../main.c
./a.out
ffmpeg -i %d.png -pix_fmt yuv420p output.mp4
rm *.png
mv output.mp4 ../videos/output.mp4
cd ..
rm -rf tmp_dirx