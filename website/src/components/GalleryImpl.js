import React from "react"

class GalleryImpl extends React.Component {
  constructor(props) {
    super(props)
    this.state = {
      images_urls: [],
    }
  }

  componentDidMount() {
    fetch("https://res.cloudinary.com/coollab/image/list/gallery.json")
      .then((response) => response.json())
      .then((data) => {
        const images_urls = data.resources.map(
          (info) =>
            `https://res.cloudinary.com/coollab/image/upload/v${info.version}/${info.public_id}.${info.format}`
        )
        this.setState({ ...this.state, images_urls })
      })
  }

  render() {
    const images = this.state.images_urls.map((url) => {
      return <img src={url} style={{ height: "50px", margin: "2px" }}></img>
    })

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
        <div>{images}</div>
      </div>
    )
  }
}

export default GalleryImpl
