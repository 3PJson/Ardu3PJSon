# ArduinoPPJson

**ArduinoPPJson** is a library to use **PPJson** custom protocol, in combination with other Arduinos or with the **pyPPJson** library on a computer or raspberry Pi.

ArduPPPJS is a library to use PPPJS over Arduino streams : a custom 2in1 protocol that offers a versatile mean of exchange for fast, reliable, self-checked data communication with "Point to Point Protocol" (PPP) like frames, as well as the famous Json key value pairs exchange to make humanly readble configuration messages and queries to your most ambitious Arduino projects.

It is a versatile protocol that can fit almost any advanced usage of **Arduino** serial communication, or any other **C++ compiler compatible microcontroller**. It supports **simultaneous** use of config heavy and nicely **maintainable protocol**, with Json flavored key values pairs easily readable by a human, and binary transfer, for **fast and efficient high data throughput** communication, while making both very reliable with strong and **user transparent sanity checks**, to keep ease of use and abstraction layer out of the user's mind, allowing them to focus on the content without worrying about data loss or maintainability.

It is designed for **microcontroller to microcontroller** or **microcontroller to computer** communication. It will support data routing in near future for multiple microcontroller networks.

The UART is located at the **Data Link Layer** on the [OSI model layer 2](https://en.wikipedia.org/wiki/Data_link_layer).



Versatile communication protocol based :

----------

- For fast transfer on :

HDLC : [High-Level Data Link Control Protocol](https://en.wikipedia.org/wiki/High-Level_Data_Link_Control)

PPP : [Point-to-Point Protocol](https://en.wikipedia.org/wiki/Point-to-Point_Protocol#PPP_frame)

![https://www.rfwireless-world.com/images/HDLC-Frame-Structure.jpg](C:\Users\Timothe\NasgoyaveOC\Professionnel\TheseUNIC\MKdocs\notedocs\Images\Blog_Notes\HDLC-Frame-Structure.jpg)

---------

- For highly versatile and humanly readable values on :

[(relaxed)](http://www.relaxedjson.org/) Json : [JavaScript Object Notation](https://fr.wikipedia.org/wiki/JavaScript_Object_Notation) based on the wonderfull [ArduinoJson](https://github.com/bblanchon/ArduinoJson) library by [Benoit Blanchon](https://github.com/bblanchon).



-------

- Arduino Library Manager inclusion : (https://github.com/arduino/Arduino/wiki/Library-Manager-FAQ)



[Hjson : converting user "dirty" json into json valid syntax.](https://hjson.github.io/)

## Rules :

- Start of a packet should be **{**

- Json keys should **NOT** start with numbers




