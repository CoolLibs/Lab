import React from "react"

export default ({ name, github_name, link, roles }) => {
  return (
    <span>
      <b>
        <a href={link} target="_blank">
          <img
            src={`https://avatars.githubusercontent.com/${github_name}`}
            width={20}
          />{" "}
          {name}
        </a>
      </b>
      , <i>{roles.join(" | ")}</i>
    </span>
  )
}
