import { Navigate, Route, Routes } from 'react-router-dom'
import ScrollToTop from './components/ScrollToTop'
import DocumentTitle from './components/DocumentTitle'
import Landing from './pages/Landing/Landing'
import BrandShell from './pages/Home/BrandShell'
import HomeView from './pages/Home/HomeView'
import AboutView from './pages/AboutMe/AboutView'
import Game from './pages/Game/Game'
import AI from './pages/AI/AI'
import Section from './pages/Section/Section'
import NotFound from './pages/NotFound/NotFound'
import LensPlayground from './playground/lens/LensPlayground'

function App() {
  return (
    <>
      <DocumentTitle />
      <Routes>
        <Route path="/" element={<Navigate to="/home" replace />} />
        <Route element={<ScrollToTop />}>
          <Route path="/intro" element={<Landing />} />
          {/* Keep the brand mounted while Home and About transition. */}
          <Route element={<BrandShell />}>
            <Route path="/home" element={<HomeView />} />
            <Route path="/about-me" element={<AboutView />} />
          </Route>
          <Route path="/game" element={<Game />} />
          <Route path="/ai" element={<AI />} />
          <Route path="/lens" element={<LensPlayground />} />
          <Route path="*" element={<NotFound />} />
        </Route>
        {/* Section sets its own entry scroll position, so it stays outside ScrollToTop. */}
        <Route path="/ai/:slug/:child?" element={<Section category="ai" />} />
        <Route path="/game/:slug/:child?" element={<Section category="game" />} />
      </Routes>
    </>
  )
}

export default App
