import React from "react"
import Layout from "@theme/Layout"

class docApi extends React.Component {
  constructor(props) {
    super(props)
    this.state = {
      text: "yoo",
    }
  }

  componentDidMount() {
    const axios = require("axios")

    // Make a request for a user with a given ID
    // axios
    //   .get("https://sore-lime-anemone-wear.cyclic.app/")
    //   .then(function (response) {
    //     // handle success
    //     console.log(response)
    //   })
    //   .catch(function (error) {
    //     // handle error
    //     console.log(error)
    //   })
    //   .finally(function () {
    //     // always executed
    //   })

    // console.log("sdf")
    fetch("https://sore-lime-anemone-wear.cyclic.app/")
      .then((response) => response.json())
      .then((data) => this.setState({ text: `${data[0].name}` }))
    //   .then((error) => {
    //     console.error(error)
    //   })
    // console.log(data)
    // const data = fetch("https://coollab-gallery.onrender.cm/")
    //   .then((response) => {
    //     console.log(this.text)
    //     this.text = response
    //     console.log(this.text)
    //     this.render()
    //   })
    //   .catch((error) => {
    //     console.error(error)
    //   })
  }
  componentWillUnmount() {
    if (typeof window !== "undefined") {
      window.location.reload()
    }
  }
  render() {
    const { text } = this.state
    return (
      <div
        style={{
          display: "flex",
          justifyContent: "center",
          alignItems: "center",
          height: "50vh",
          fontSize: "20px",
        }}
      >
        <p>🚧 COMING SOON 🚧</p>
        <p>{text}</p>
      </div>
    )
  }
}

export default docApi
