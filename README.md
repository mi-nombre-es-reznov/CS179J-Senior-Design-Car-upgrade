<h1 align="center">
  Archived
</h1>


# CS179J Senior Design - Secure Fridge
<h2>Project Description</h2>
<h3>Link to Demonstration <a href="https://youtu.be/trOnz5R9IiI" target="_blank">here</a></h3>
<p>
  Our project, Secure Fridge, will act as a secure fridge for all products that you typically have in a regular fridge. Our system will keep track of your product quantity, require a two-step authentication to access the contents, and will host a website in which it will update from a database with the new current values after every transaction. The system will have several methods of inputs. There is a specific process that the user must also go through in order to navigate their transactions successfully.
</p>
<p>
  The process begins with a user identifying themselves through a fingerprint scanner. If the user has been found within the system, then the user will have to enter their preset pin as a 2-step authentication. Once the user has been identified by the two-step authentication phase, a solenoid lock will unlock. Upon the lock unlocking, the identified user will be allowed to begin scanning their products that they want to insert/remove. After they are completely finished, they will complete the transaction by scanning a ‘Lock’ barcode which will lock the container and update the data.
</p>
<p>
  Once the container locks, there is a weight sensor that has been keeping track of weights and items that were added/removed during the process. Once the items in the database, they are then read into a website. The website will display the data of each item and will hold all data that is from the database on it for the user to see. This website will update after every transaction and will always be up to date as long as there is internet connected to the Raspberry Pi.
</p>

<h2>Path to Completion</h2>
<p><b> As you continue to scroll down, you will see our progression throughout this project. There will be images showing what we have chosen to use in our project and their respective growth with time.</b></p>

![Untitled_Diagram_1](https://user-images.githubusercontent.com/32404479/58624954-410b5100-8286-11e9-9b3c-911331353a88.png)

<p>This image shows how we expect someone to identify themselves, gain access to the contents inside, commit to transaction(s), and then close the container. The system is going to continually loop itself.</p>

![Fig-3](https://user-images.githubusercontent.com/32404479/58625343-238ab700-8287-11e9-84c6-2cb469dd6345.png)

<p>For this project, we plan on using a Raspberry Pi...</p>

![71xQcrU2NkL _SX679_](https://user-images.githubusercontent.com/32404479/58625413-4c12b100-8287-11e9-95c4-d2c4ebd15a15.jpg)

<p>... And an Elegoo. These two devices will establish communication via UART. The Elegoo will host the fingerprint scanner with its own internal database, while everything else will be run from the Raspberry Pi.</p>

<p>The Elegoo will run off of Arduino code [a form of the C language] with its dedicated hardware for mapping General Purpose inputs and outputs (GPIO).</p>

![cvxzdf](https://user-images.githubusercontent.com/32404479/58625632-b592bf80-8287-11e9-83b9-f18a9dea228f.png)

<p>The Raspberry Pi [using a form of the Python language] will run off a set mapping of GPIO, based on the chosen settings.</p>

![20190427_155744](https://user-images.githubusercontent.com/32404479/58626127-e32c3880-8288-11e9-967a-0315729dc333.jpg)

<p>Our 'Secure Fridge' is going to be this box. This is where the weight sensor and all the products will go.</p>

![20190427_160012](https://user-images.githubusercontent.com/32404479/58626213-18d12180-8289-11e9-846f-9df6f0b401ba.jpg)

<p>The weight sensor is going to be relatively small compared to the box, so we are going to attach this to it. This will allow for the products to be placed on top of it so that the weight sensor can accurately calculate all of the products.</p>

![20190427_155402](https://user-images.githubusercontent.com/32404479/58626300-4d44dd80-8289-11e9-8308-e8c4f49cae9e.jpg)

<p>All of the products will be placed inside of the box, along with the weight sensor at the bottom and solenoid lock towards the top.</p>

![20190427_160152](https://user-images.githubusercontent.com/32404479/58626346-749baa80-8289-11e9-8586-2524ddfc0b9b.jpg)

<p>This will be achieved through the use of drilling and mounting of components together.</p>

![2776-05](https://user-images.githubusercontent.com/32404479/58626398-9301a600-8289-11e9-8822-59c167be6d83.png)

<p>The solenoid lock is going to be small relative to the box. This allows for more products to be stored without interference from the solenoid 'lock'.</p>

<h2>Current Status - Week 8 of 10</h2>
<p>We are currently at the point where we are finalizing code to work together in our core. After that, we will map out on the box where all of the components are to be placed.</p>

![Image of Secure Fridge](https://user-images.githubusercontent.com/32404479/58626636-1ae7b000-828a-11e9-8794-4d088f879a6d.png)

<p>This representation is one example of what the final product may look like</p>

<h1>Final Project - Integrated<h1>
  
![project2](https://user-images.githubusercontent.com/32404479/59477565-9446dd00-8e0a-11e9-8fa4-df3101984b22.jpg)

![box_2](https://user-images.githubusercontent.com/32404479/59477575-a45ebc80-8e0a-11e9-96e3-dcc84da4cc03.jpg)

<p>A couple of images of our project with integration!</p><br>

<h1>Thank you for viewing our CS 179J: Senior Design Project</h1>

![finalprojectgrouppic1](https://user-images.githubusercontent.com/32404479/59477581-b4769c00-8e0a-11e9-891e-dba2ee3bd7e0.png)
