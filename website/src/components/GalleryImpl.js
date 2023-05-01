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
    const images = this.state.images.map((image, i) => {
      return (
        <div className="gallery-frame" key={i}>

          <img src={image.url} className="gallery-img" alt=""></img>

           <div className="gallery-infos">

              <h2>Title {image.title} </h2>
              <h3> by <a href={image.author_link} target="_blank" className="site-button" >{image.author_name}</a></h3>
              Description : {image.description} <br></br>

          </div>

        </div>
      )
    })

    return (
    <div>

      <div className="gallery-header">
        <h2>Discover all of the community's incredible art!</h2>
        <h3><i>Hover to know more 🖱️</i></h3>
      </div>

      <div className="gallery-impl">{images}</div>

    </div>
    )
  }
}

export default GalleryImpl
