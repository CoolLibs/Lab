import BrowserOnly from "@docusaurus/BrowserOnly"
import React from "react"

// https://stackoverflow.com/a/12646864/15432269
function shuffleArray(array) {
  for (var i = array.length - 1; i > 0; i--) {
    var j = Math.floor(Math.random() * (i + 1))
    var temp = array[i]
    array[i] = array[j]
    array[j] = temp
  }
}

export default ({ elements }) => {
  shuffleArray(elements)
  return (
    <BrowserOnly>
      {() => (
        <ul>
          {elements.map((element) => (
            <li>{element}</li>
          ))}
        </ul>
      )}
    </BrowserOnly>
  )
}
