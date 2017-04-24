# EarthquakeVisualization

Authors: Divino Kenneth Bello and Kohei Hisakuni
----------------------------------------------------------------------------------------------------------------------------
![alt tag](https://github.com/kennybello/EarthquakeVisualization/blob/master/Capture4.png)

We used the formulas  x = r * sin(phi) * sin(theta), y = r * cos(theta), z = r * cos(phi) * sin(theta), to convert the spherical coordinates to Cartesian coordinates. Using this formula we were able to tesselate our sphere starting from the north pole or (0,1,0). We also used the triangle primitive type and used the points where the slices and stacks intersect to create the individual triangles. 

![alt tag](https://github.com/kennybello/EarthquakeVisualization/blob/master/Capture8.png)

To display the earthquake data, we used a green to red color range, green for lower magnitudes, and red for higher ones. We also increased the radius of the circles for each earthquake based on the magnitude and placed them on the earth based on their longitude and latitude. 

![alt tag](https://github.com/kennybello/EarthquakeVisualization/blob/master/Capture7.png)

![alt tag](https://github.com/kennybello/EarthquakeVisualization/blob/master/Capture5.png)
