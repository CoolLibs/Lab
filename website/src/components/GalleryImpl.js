import React from "react";
// import {Carousel} from '3d-react-carousal';
import Link from "@docusaurus/Link";

class GalleryImpl extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      images: [],

      Opened: false,
      currentImageIndex: null,
      previousImageIndex: [],
      nextImageIndex: [],
      index: 0,

      animate: false,
    };
  }

  componentDidMount() {
    fetch("https://res.cloudinary.com/coollab/image/list/gallery.json")
      .then((response) => response.json())
      .then((data) => {
        const images = data.resources.map((info) => {
          const metadata =
            info.context !== undefined ? info.context.custom : {};
          return {
            url: `https://res.cloudinary.com/coollab/image/upload/v${info.version}/${info.public_id}.${info.format}`,
            title: metadata.title || "Untitled",
            description: metadata.description || "",
            author_name: metadata.author_name || "",
            author_link: metadata.author_link || "",
          };
        });
        this.setState({ ...this.state, images });
      });

    document.addEventListener("keydown", this.handleKeyDown);
    document.addEventListener("click", this.handleClick);
  }

  componentWillUnmount() {
    document.removeEventListener("keydown", this.handleKeyDown);
  }

  // Possible evolution: if we had a type "favorite" in metadata, we could easily display the best pictures!
  // generateCarouselImagesTag() {
  //     const images = this.state.images.slice(0, 5);
  //     return images.map((image, index) => (
  //         <img key={index} src={image.url} alt={image.title}/>
  //     ));
  // }

  openImg = (index) => {
    this.setSurroundingsFullScreenImages(index);
    this.setState({ Opened: true });
  };

  setSurroundingsFullScreenImages = (index) => {
    this.setState({
      previousImageIndex:
        index - 1 < 0 ? this.state.images.length - 1 : index - 1,
      currentImageIndex: index,
      nextImageIndex: index + 1 > this.state.images.length - 1 ? 0 : index + 1,
    });
  };

  setPrevFullScreenImage = () => {
    this.setSurroundingsFullScreenImages(this.state.previousImageIndex);
  };

  setNextFullScreenImage = () => {
    this.setSurroundingsFullScreenImages(this.state.nextImageIndex);
  };

  getCurrentFullScreenImage = () => {
    return this.state.images[this.state.currentImageIndex].url;
  };

  closeImg = () => {
    this.setState({ animate: true }, () => {
      setTimeout(() => {
        this.setState({ Opened: false, fullImageSrc: null, animate: false });
      }, 400); /* Duration of the animation in milliseconds */
    });
  };

  handleKeyDown = (event) => {
    if (this.state.Opened) {
      if (event.key === "Escape") this.closeImg();
      if (event.key === "ArrowLeft") this.setPrevFullScreenImage();
      if (event.key === "ArrowRight") this.setNextFullScreenImage();
    }
  };

  handleClick = (event) => {
    if (this.state.Opened) {
      const openingImg = [...document.querySelectorAll('.gallery-img')]
      const leftButton = document.querySelector('.prev-button')
      const rightButton = document.querySelector('.next-button')
      const closeButton = document.querySelector('.close-button')

      if (openingImg.includes(event.target) || event.target === leftButton || event.target === rightButton || event.target === closeButton) {
        return;

      }
      this.closeImg()
    }
  };

  render() {
    const images = this.state.images.map((image, i) => {
      return (
        <div className="gallery-frame" key={i}>
          <img
            src={image.url}
            className="gallery-img"
            alt=""
            onClick={() => this.openImg(i)}
          ></img>

          <div className="gallery-infos">
            <h2>{image.title}</h2>
            <h3>
              {image.author_name || image.author_link ? (
                <span>
                  by{" "}
                  {image.author_link ? (
                    <a href={image.author_link} target="_blank">
                      {image.author_name || "Unknown"}
                    </a>
                  ) : (
                    image.author_name || "Unknown"
                  )}
                </span>
              ) : null}
            </h3>

            {image.description && <>{image.description}</>}
            <br></br>
          </div>
        </div>
      );
    });

    return (
      <div className="gallery">
        <h2>Discover all of the community's incredible art!</h2>
        <h3>
          <i>
            Submit your own artwork with <Link to="/Download">Coollab️</Link> 🎨
          </i>
        </h3>
        {/*<Carousel*/}
        {/*    slides={this.generateCarouselImagesTag()}*/}
        {/*    autoplay={true}*/}
        {/*    interval={3000}*/}
        {/*/>*/}
        {/*<div className="gallery-header">*/}
        {/*    <h3><i>Hover to know more 🖱️</i></h3>*/}
        {/*</div>*/}
        <div
          className="gallery-impl"
          style={{ pointerEvents: this.state.Opened ? "none" : "auto" }}
        >
          {/* So as we cannot click on other images behind when one full screen */}
          {images}
        </div>

        {this.state.Opened && (
          <div className="img-overlay">
            <img
              src={this.getCurrentFullScreenImage()}
              className={
                this.state.animate ? "animate-scale-down" : "full-image"
              }
              alt="Fullscreen image"
            />
            <i className="close-button fa fa-times" onClick={this.closeImg}></i>
            <i
              className="prev-button fa fa-arrow-left"
              onClick={this.setPrevFullScreenImage}
            ></i>
            <i
              className="next-button fa fa-arrow-right"
              onClick={this.setNextFullScreenImage}
            ></i>
          </div>
        )}
      </div>
    );
  }
}

export default GalleryImpl;
