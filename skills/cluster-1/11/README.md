Author: Eric Chen

Date: 9/19/23

Summary:
I have 3 tasks running at the same time. The tasks are: counting, reading the state of the push button, and lighting up the blue LED based on the counting direction. Counting has the highest priority and the other two tasks have a lower priority but the same priority as each other. The counting task has a 1 second delay, the button task has a 150ms delay to mimic the speed that a human finger could press the button, and the blue LED task has a 50ms delay since I want it to be very responsive relative to the possible change to the counting direction. 

Circuit:
![circuit](https://github.com/BU-EC444/Chen-Eric/assets/98416392/a8de5334-3e75-4e39-8ffe-9deab8f39268)

Video:
[Video Demonstration](https://drive.google.com/file/d/1dvxFQBSUOLveea4vilUkrTPJ3h7eVK3t/view?usp=drive_link)
