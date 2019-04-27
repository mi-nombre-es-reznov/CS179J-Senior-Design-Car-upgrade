# CS179J Senior Design - Secure Fridge
This will be our senior design project.

<h1>Project Description</h1>
<p>
  Our project, Secure Fridge, will act as a secure fridge for all products that you typically have in a regular fridge. Our system will keep track of your product quantity, require a two-step authentication to access the contents, and will host a website in which it will update from a database with the new current values after every transaction. The system will have several methods of inputs. There is a specific process that the user must also go through in order to navigate their transactions successfully. 
  The process begins with a user identifying themselves through a fingerprint scanner. If the user has been found within the system, then the user will have to enter their preset pin as a 2-step authentication. Once the user has identified by the two-step authentication phase, a solenoid lock will unlock. Upon the lock unlocking, the identified user will be allowed to begin scanning their products that they want to insert/remove. After they are completely finished, they will complete the transaction by scanning a ‘Lock’ barcode which will lock the container and update the data.
  Once the container locks, there is a weight sensor that has been keeping track of weights and items that were added/removed during the process. This process will update the weights accordingly and then it will take a ranged average of weights for each item selected and quantify them. Upon quantifying the items, the data will be sent to a database for further actions.
Once the items in the database are quantified and transferred, they are then read into a website. The website will display the data of each item and will hold all data that is from the database on it for the user to see. This website will update after every transaction and will always be up to date as long as there is internet connected to the Raspberry Pi.
</p>
