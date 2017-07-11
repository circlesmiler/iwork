# iWork - Work Time Recognizing on Particle Photon
This is a Particle Photon project. The idea is to provide a simple display
and a login and logout button to recognize the start work time end the end
of work.

On the display, you can see following informations:
* Start of Work (hh:mm)
* End of Work (hh:mm)
* Time at Work (hh:mm)
* Current Time (hh:mm)
* WIFI strength (as Image)
* Outside Temperature (OpenWeather API)

![alt tag](https://raw.githubusercontent.com/circlesmiler/iwork/master/iWork-fritzing.png)

## Login and Logout
It's possible to login and logout due to
* Pressing the login or logout button
  * Publishes an Event _work_state_ with folling data:
    * Start|||01.01.2017 07:00
    * Ende|||01.01.2017 17:00
* Using the IFTTT Location
  * Trigger: Mobile Phone Location (IFTTT app needed on the phone)
  * Action: Particle Publish Event
    * Event == "on_work"
    * Data == %EnteredOrExited%
* Using the IFTTT button widget (on phone)
  * Trigger: Button widget
  * Action: Particle Publish Event
    * Event == "on_work"
    * Data == "entered" or "exited"

For a better usability, I used buttons with integrated LEDs, that can be
controlled separately. So, the *Login Button* glows GREEN, when the user is
currently not logged in. The LED turns of, when the user pressed the
*Login Button*. The *Logout Button* only glows RED, when the user is currently
logged in. Very easy, isn't it? ;-)

<a href="http://www.youtube.com/watch?feature=player_embedded&v=zKBNXiXqFS4" target="_blank">
<img src="http://img.youtube.com/vi/zKBNXiXqFS4/0.jpg" alt="iWork at work" width="240" height="180" border="10" />
</a>

## Weather Configuration (City ID)
The temperature shown on the display is provided by the OpenWeather API. To
get the temperature value of your city, you can call the Particle function
_owmCityId_ to set your city id.

See also http://openweathermap.org/help/city_list.txt

## The Mode Button
A click on the **Mode Button** changes the page on the display. A long click on
the **Mode Button** changes to a **setup mode**. Currently the **setup mode** is
only a dummy function. The idea is to UNDO a wrong login or logout due to the
IFTTT actions.
