import { useEffect, useState } from 'react'
import './App.css'

const API_BASE = import.meta.env.VITE_API_BASE || '/api'

function App() {
  const [status, setStatus] = useState('loading')
  const [tick, setTick] = useState(0)
  const [engine, setEngine] = useState('python')
  const [tileCount, setTileCount] = useState(0)
  const [tiles, setTiles] = useState([])
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState('')

  const loadHealth = async () => {
    try {
      const res = await fetch(`${API_BASE}/health`)
      const json = await res.json()
      setStatus('ok')
      setTick(json.tick ?? 0)
      setEngine(json.engine ?? 'python')
      setError('')
    } catch (err) {
      setStatus('error')
      setError(err.message || 'Cannot reach API')
    }
  }

  const doTick = async () => {
    setLoading(true)
    try {
      const res = await fetch(`${API_BASE}/tick`, { method: 'POST' })
      const json = await res.json()
      setTick(json.tick ?? tick)
      setEngine(json.engine ?? engine)
      setError('')
    } catch (err) {
      setError(err.message || 'Tick failed')
    } finally {
      setLoading(false)
    }
  }

  const loadTiles = async () => {
    setLoading(true)
    try {
      const res = await fetch(`${API_BASE}/tiles`)
      const json = await res.json()
      setTileCount(json.tile_count ?? 0)
      setTick(json.tick ?? tick)
      setTiles(json.tiles ?? [])
      setError('')
    } catch (err) {
      setError(err.message || 'Failed to load tiles')
    } finally {
      setLoading(false)
    }
  }

  useEffect(() => {
    loadHealth()
  }, [])

  const topTiles = tiles.slice(0, 8)

  return (
    <div className="app-shell">
      <header className="hero">
        <div>
          <p className="eyebrow">Living Planet Simulator</p>
          <h1>Planetary Research Engine</h1>
          <p className="subtitle">Scientific-scale planet simulation with C++ core and React client.</p>
        </div>
        <div className="status-chip">Engine: <strong>{engine}</strong></div>
      </header>

      <section className="controls">
        <div className="card">
          <div className="grid">
            <div>
              <p className="label">Simulation tick</p>
              <p className="value">{tick}</p>
            </div>
            <div>
              <p className="label">Tile count</p>
              <p className="value">{tileCount}</p>
            </div>
            <div>
              <p className="label">Status</p>
              <p className={`value ${status === 'ok' ? 'good' : 'bad'}`}>{status}</p>
            </div>
          </div>
          <div className="actions">
            <button onClick={loadHealth}>Refresh</button>
            <button onClick={doTick} disabled={loading}>Advance Tick</button>
            <button onClick={loadTiles} disabled={loading}>Load Tiles</button>
          </div>
          {error && <p className="error">{error}</p>}
        </div>
      </section>

      <section className="tile-grid">
        <div className="header-row">
          <h2>Live tile snapshot</h2>
          <span>{topTiles.length} tiles displayed</span>
        </div>
        <div className="grid-cards">
          {topTiles.map((tile) => (
            <div className="tile" key={tile.id}>
              <div className="tile-header">
                <span># {tile.id}</span>
                <span>{tile.is_ocean ? 'Ocean' : 'Land'}</span>
              </div>
              <div className="tile-value">ELEV<br/><strong>{tile.elevation.toFixed(2)}</strong></div>
              <div className="tile-value">TEMP<br/><strong>{tile.temperature.toFixed(1)}°C</strong></div>
              <div className="tile-value">HUM<br/><strong>{tile.humidity.toFixed(2)}</strong></div>
              <div className="tile-value">POP<br/><strong>{tile.population.toFixed(2)}</strong></div>
            </div>
          ))}
        </div>
      </section>

      <footer className="footer">Built for open-source planetary simulation. Upload-ready client + C++ backend.</footer>
    </div>
  )
}

export default App
