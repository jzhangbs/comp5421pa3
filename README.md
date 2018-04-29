# COMP5421 Project 3
This project implements _Single View Metrology_ by Criminisi, Reid, and Zisserman. 

## Algorithm Description
### Calculate Vanishing Points and Lines
You need to manually draw lines parallel to X, Y and Z axis respectively. Then the program will calculate the vanishing points and lines in the image coordinates. 

### Set Coordinate System and Reference Length
You need to manually set the world coordinate system in the image. Click the image 4 times to set the origin, a point on X axis, Y axis and Z axis. You also need to input 3 length of the line from the point on each axis to the origin. 

### Choose Interest Points and Corresponding Projection
Indicate the 4 points clockwise of the polygon that will be put in 3D model. For each point, you also need to set its projection on one of XY, YZ or XZ plane. Click 8 times to set point 1, projection of point 1, point 2, projection of point 2, etc. 

### Compute Texture Map
The program will wrap and crop the image to get the texture map of the polygons. 
