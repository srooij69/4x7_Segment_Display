# 4x7_Segment_Display
Lesson 28 - 4 Digit 7 Segment Display


Ricardo Moreno has created a great tutorial on how to use a 4 digit 7 segment display with arduino.
He improved the original circuit and Arduino sketch. (see his tutorial https://www.youtube.com/watch?v=gesrM2J8VnY )

I think the code can even be improved even more.
A big leap can be made by separating the plumbing of getting digits to show on the display from the logic to what to display.

I used a char[4] buffer for this because both digits and characters need to be shown.
There is 1 function that handles the display of the values in the buffer.
And there is another function that manages the content of the buffer.

For this to work each function, regardless of timing, may never block the general flow. 
The only way to accomplish that is to stop using the delay functions.
see both:
- https://www.norwegiancreations.com/2017/09/arduino-tutorial-using-millis-instead-of-delay/
- https://www.norwegiancreations.com/2018/10/arduino-tutorial-avoiding-the-overflow-issue-when-using-millis-and-micros/

I provide 2 versions:
- one that is similar to Ricardo's version.
- An extended version because having this separation i was tempted to play with some variations :-)
     like scrolling the text 'halo' across the display.
     
Actually, using a timer interrupt would even work better still. But I think interrupts are beyond the scope of the tutorial.
