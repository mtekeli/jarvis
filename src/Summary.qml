import QtQuick 2.0

Row {
    id: root

    property string fontName

    readonly property var weatherService: App.weatherService
    readonly property var curentWeather: weatherService ? weatherService.currentWeather : null

    // thermometer
    DGauge {
        imageSource: "assets/svg/thermometer.svg"
        primaryTextSize: 60
        fontName: fontName
        primaryText: RoomService.temperature ? RoomService.temperature.real : primaryText
        secondaryText: RoomService.temperature ? RoomService.temperature.decimals.substring(0,1) + "°" : secondaryText
    }

    // humidity
    DGauge {
        imageSource: "assets/svg/humidity.svg"
        primaryTextSize: 60
        fontName: fontName
        primaryText: RoomService.humidity ? RoomService.humidity.real : primaryText
        secondaryText: RoomService.humidity ? RoomService.humidity.decimals.substring(0,1) : secondaryText
    }

    // forecast (TODO)
    DGauge {
        imageSource: curentWeather ? "assets/svg/weather/"+curentWeather.weather+".svg" : "assets/svg/sun.svg"
        colorize: true
        color: "white"
        primaryTextSize: 60
        fontName: fontName
        primaryText: curentWeather ? curentWeather.temperature.real: primaryText
        secondaryText: curentWeather ? curentWeather.temperature.decimals.substring(0,1) + "°" : secondaryText
    }
}
