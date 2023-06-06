import React from "react"
import styles from "./YoutubeVideo.module.css"

export default function YoutubeVideo({ id }) {
  return (
    <div className={styles.video}>
      <iframe
        src={`https://www.youtube.com/embed/${id}`}
        title="YouTube video player"
        frameborder="0"
        allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
        allowfullscreen=""
      ></iframe>
    </div>
  )
}
