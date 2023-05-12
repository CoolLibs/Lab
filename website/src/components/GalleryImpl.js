import React from "react";
import {Carousel} from '3d-react-carousal';
import Link from '@docusaurus/Link';

class GalleryImpl extends React.Component {
    constructor(props) {
        super(props)
        this.state = {
            images: [],

            currentImage: [],
            setCurrentImage: [],

            Opened: false,
            fullImageSrc: null,
        };
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
                        title: metadata.title || "Untitled",
                        description: metadata.description || "",
                        author_name: metadata.author_name || "",
                        author_link: metadata.author_link || "",
                    }
                })
                this.setState({...this.state, images})
            })

        document.addEventListener('keydown', this.handleKeyDown);
    }

    componentWillUnmount() {
        document.removeEventListener('keydown', this.handleKeyDown);
    }

    // Possible evolution: if we had a type "favorite" in metadata, we could easily display the best pictures!
    generateCarouselImagesTag() {
        const images = this.state.images.slice(0, 5);
        return images.map((image, index) => (
            <img key={index} src={image.url} alt={image.title}/>
        ));
    }

    openImg = (imageSrc) => {
        this.setState({Opened: true, fullImageSrc: imageSrc});
    };

    closeImg = () => {
        this.setState({Opened: false, fullImageSrc: null});
    };

    handleKeyDown = (event) => {
        if (event.key === 'Escape' && this.state.Opened) {
            this.closeImg();
        }
    };

    render() {
        const images = this.state.images.map((image, i) => {
            return (
                <div className="gallery-frame" key={i}>

                    <img src={image.url} className="gallery-img" alt="" onClick={() => this.openImg(image.url)}></img>

                    <div className="gallery-infos">

                        <h2>{image.title}</h2>
                        <h3>
                            {image.author_link ? (
                                <>
                                    by <a href={image.author_link} target="_blank">{image.author_name}</a>
                                </>
                            ) : (
                                ""
                            )}
                        </h3>

                        {image.description ? (
                            <>
                                Description: {image.description}
                            </>
                        ) : (
                            ""
                        )}
                        <br></br>
                        <i>Click to display fullscreen! 🖱️</i>

                    </div>

                </div>
            )
        })

        return (
            <div className="gallery">
                <h2>Discover all of the community's incredible art!</h2>
                <h3><i>Submit your own artwork on <Link to="/Download">Coollab️</Link> 🎨</i></h3>
                <Carousel
                    slides={this.generateCarouselImagesTag()}
                    autoplay={true}
                    interval={3000}
                />
                <div className="gallery-header">
                    <h3><i>Hover to know more 🖱️</i></h3>
                </div>
                <div className="gallery-impl"
                     style={{pointerEvents: this.state.Opened ? "none" : "auto"}}>
                    {/* So as we cannot click on other images behind when one full screen */}
                    {images}
                </div>

                {this.state.Opened && (
                    <div className="img-overlay" onClick={this.closeImg}>
                        <img
                            src={this.state.fullImageSrc}
                            className="full-image"
                            alt="Fullscreen image"
                        />
                        <i className="close-button fa fa-times"
                           onClick={this.closeImg}></i>
                    </div>
                )}

            </div>
        );
    }
}

export default GalleryImpl;
