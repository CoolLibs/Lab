import React from "react"

class GalleryImpl extends React.Component {
  constructor(props) {
    super(props)
    this.state = {
      images: [],
    }
  }

  componentDidMount() {
    fetch("https://res.cloudinary.com/coollab/image/list/gallery.json")
      .then((response) => response.json())
      .then((data) => {
          const images = data.resources.map((info) => {
            const metadata =
              info.context !== undefined ? info.context.custom : {}
            return {
              url: `https://res.cloudinary.com/coollab/image/upload/v${info.version}/${info.public_id}.${info.format}`,
              title: metadata.title || "",
              description: metadata.description || "",
              author_name: metadata.author_name || "",
              author_link: metadata.author_link || "",
            }
          })
          this.setState({ ...this.state, images })
      })
  }

  render() {
    const images = this.state.images.map((image) => {
      return (
        <span>
          <img src={image.url} style={{ height: "50px", margin: "2px" }}></img>
          <caption>
            <div>title: {image.title}</div>
            <div>description: {image.description}</div>
            <div>author_name: {image.author_name}</div>
            <div>author_link: {image.author_link}</div>
          </caption>
        </span>
      )
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
