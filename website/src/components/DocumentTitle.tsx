import { useEffect } from 'react'
import { useLocation } from 'react-router-dom'
import { routeMeta } from '../data/routes'

export default function DocumentTitle() {
  const { pathname } = useLocation()
  useEffect(() => {
    document.title = routeMeta(pathname).title
  }, [pathname])
  return null
}
