Steps for compilation
1 - Open up a terminal window in the "build" folder
2 - Enter "make" in the terminal window to build the project
3 - Enter "./coaster" to run the program

Press space bar to view from a different angle. 
(I couldn't get first person view to work. I thought I just
needed to set the camera/view matrix to the cart's model matrix.
But it didn't work.)

Explanation of Math/physics

Basically I just followed the roller coaster pdf.

To create the curve, I subdivided the curve created from the control points
until it seemed smooth enough.

I then chose a small change in distance and stepped through my curve, while
keeping track of the current distance travelled. Once the distance travelled 
was greater than my chosen change in distance, I recorded the point I was at 
so that I could arc-length parameterize my curve.

Now that my curve is arc length parameterized, I can choose a constant change
in time to use to animate the motion of the cart. Now I just calculated the speed
of the cart using the conservation of energy formula. With the speed now figured out
and constant change in time, I can figure out how far the cart moved in this change 
in time. (ds = dt * dv)

With the change in distance calculated, I can divide it by the change in distance 
of the segements of my curve to find out how far along my curve I should travel.
This takes care of the motion of my cart.

To properly orientate the cart, I just set its model matrix to have the tangent/velocity
as it's x-axix, the normal as it's y-axis, and the cross product of these to axis as the
z-axis. The normal was calculated by adding the approximated centripetal acceleration at
the carts current point with -g = 9.81m/s^2.

Resources
http://algorithmicbotany.org/courses/CPSC587/Winter2018/CPSC587_687_RollerCoaster.pdf
http://algorithmicbotany.org/courses/CPSC587/Winter2018/CPSC587-2010W-04.pdf
https://www.khanacademy.org/science/physics/centripetal-force-and-gravitation/centripetal-acceleration-tutoria/v/centripetal-force-and-acceleration-intuition
