import React from "react"
import Layout from "@theme/Layout"
import BrowserOnly from "@docusaurus/BrowserOnly"

import loadable from "@loadable/component"
const Impl = loadable(() => import("../components/GalleryImpl"))

export default function () {
  return (
    <Layout title="Hello" description="Hello React Page">
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
        <BrowserOnly>{() => <Impl />}</BrowserOnly>
      </div>
    </Layout>
  )
}
