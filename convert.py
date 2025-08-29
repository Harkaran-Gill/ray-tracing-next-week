from PIL import Image

out_name = input("Enter file name without extension: ")

img = Image.open("image.ppm")
img.save("./RayTracedImages/ppmImages/"+out_name+".ppm")
img.save("./RayTracedImages/pngImages/"+out_name+".png")