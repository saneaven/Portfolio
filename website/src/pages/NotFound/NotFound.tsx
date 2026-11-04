import { useEffect } from 'react'
import { Link } from 'react-router-dom'
import { NOT_FOUND_TITLE } from '../../data/routes'
import './NotFound.css'

export default function NotFound() {
  useEffect(() => {
    document.title = NOT_FOUND_TITLE
  }, [])

  return (
    <main className="not-found">
      <h1 className="not-found__code">404</h1>
      <p className="not-found__message">This page doesn't exist.</p>
      <Link className="not-found__home" to="/home">
        <svg viewBox="0 0 24 24" aria-hidden>
          <path d="M15 5 L8 12 L15 19" />
        </svg>
        Home
      </Link>
    </main>
  )
}
