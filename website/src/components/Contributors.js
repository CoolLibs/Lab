import React from "react"
import Randomizedlist from "@site/src/components/RandomizedList"
import Contributor from "@site/src/components/Contributor"

export default ({}) => {
  return (
    <Randomizedlist
      elements={[
        <Contributor
          name="Jules Fouchy"
          github_name="julesfouchy"
          link="https://julesfouchy.github.io/"
          roles={["Programmer", "Project Lead"]}
        />,
        <Contributor
          name="Jolan Goulin"
          github_name="Jolan-Goulin"
          link="https://github.com/Jolan-Goulin"
          roles={["Programmer"]}
        />,
        <Contributor
          name="Emma Pernin"
          github_name="Emma-prn"
          link="https://github.com/Emma-prn"
          roles={["Programmer"]}
        />,
        <Contributor
          name="Théo Arnauld des Lions"
          github_name="Dyokiin"
          link="https://github.com/Dyokiin"
          roles={["Programmer"]}
        />,
        <Contributor
          name="Quentin Mesquita"
          github_name="MesQuentin"
          link="https://github.com/MesQuentin"
          roles={["Programmer"]}
        />,
        <Contributor
          name="Lucas David"
          github_name="MeucheRoume"
          link="http://lucasdavid.fr/"
          roles={["Programmer"]}
        />,
        <Contributor
          name="Anaïs Sarata Gougne"
          github_name="TaliaKah"
          link="https://taliakah.gitlab.io/portfolio/"
          roles={["Programmer"]}
        />,
        <Contributor
          name="Enguerrand de Smet"
          github_name="dsmtE"
          link="https://github.com/dsmtE"
          roles={["Programmer", "R&D"]}
        />,
        <Contributor
          name="Jordan Roberty"
          github_name="badbois"
          link="https://github.com/badbois"
          roles={["Programmer"]}
        />,
        <Contributor
          name="Éva Benharira"
          github_name="EvaBien"
          link="https://www.linkedin.com/in/eva-benharira-ingenieure-creative/"
          roles={["Community Management Advice"]}
        />,
        <Contributor
          name="Mattéo Leclercq"
          github_name="MatteoL-W"
          link="https://matteo-leclercq.fr/"
          roles={["Programmer", "VFX Artist"]}
        />,
        <Contributor
          name="Wendy Gervais"
          github_name="kaugrv"
          link="https://linktr.ee/commandant.grant"
          roles={["Programmer", "VFX Artist"]}
        />,
        <Contributor
          name="Romain Serres"
          github_name="Bouillon2Poulet"
          link="https://www.instagram.com/consomme2poyo/"
          roles={["Programmer", "VFX Artist"]}
        />,
        <Contributor
          name="Lou Couard"
          github_name="Couloir00"
          link="https://loucouard.wixsite.com/website"
          roles={["Programmer", "VFX Artist"]}
        />,
        <Contributor
          name="Tristan Debeaune"
          github_name="tristetemps666"
          link="https://github.com/tristetemps666"
          roles={["Programmer", "VFX Artist"]}
        />,
        <Contributor
          name="Amandine Kohlmuller"
          github_name="akohlmul"
          link="https://www.instagram.com/idnamaa/"
          roles={["UI / UX Designer"]}
        />,
        <Contributor
          name="Aurore Lafaurie"
          github_name="Just-Kiel"
          link="https://github.com/Just-Kiel"
          roles={["Programmer"]}
        />,
        <Contributor
          name="Théo Couard"
          github_name="TheoCrd"
          link="https://frankoo.artstation.com/"
          roles={["Video tutorials maker"]}
        />,
      ]}
    />
  )
}
