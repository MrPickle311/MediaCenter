import QtQuick 2.15
import "../logic/SvgGetter.js" as Logic


PlaylistDelegate {
    id: songDelegate
    mediaIcon: Logic.getControl("movie.svg")
}
