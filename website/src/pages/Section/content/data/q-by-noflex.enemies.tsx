import './q-by-noflex.enemies.css'

const ENEMIES = [
  {
    name: 'Warrior',
    body: 'Chases the last-seen player and jumps when it meets a cliff or wall, freezes into its attack at weapon range, wanders when the player is point-blank, and returns to searching once it loses sight.',
  },
  {
    name: 'FlameThrower',
    body: 'Chases like the Warrior, but stops and idles at platform edges instead of jumping so it never walks off, then attacks once the player is in range.',
  },
  {
    name: 'Scout',
    body: 'While chasing, a ~1-second timer auto-fires its attack state, launching a projectile at the player, then it slips back into the chase.',
  },
  {
    name: 'Turret',
    body: 'Never moves: it loops its attack, spawning a bullet aimed straight at the player each cycle, and drops to idle the moment it loses line of sight.',
  },
  {
    name: 'Tanker',
    body: 'Arms then defends — invincible while the shield is up — rotating to keep facing the player, and lowers the shield again when it loses sight.',
  },
  {
    name: 'Engineer',
    body: 'Runs away from the player, then summons a Turret and dies in the act — a sacrificial spawner that turns space into turret fire.',
  },
  {
    name: 'Bomber',
    body: 'Follows an A* path that re-plans every 0.1s to navigate platforms, then self-destructs on contact with a Lua-scripted explosion.',
  },
  {
    name: 'Assassin',
    body: 'A boss whose change-pattern hub picks one of four attack states at random each cycle, then loops back, throwing melee hitboxes up close.',
  },
  {
    name: 'Magician',
    body: 'A boss with the same random four-pattern hub as the Assassin, but ranged — each pattern spawns its own projectile volley.',
  },
]

export default function EnemyRoster() {
  return (
    <div className="cb__grid q-enemies">
      {ENEMIES.map((e) => (
        <div className="cb__card q-enemy" key={e.name}>
          <div className="q-enemy-thumb" />
          <div className="q-enemy-text">
            <h3 className="cb__card-title">{e.name}</h3>
            <p className="cb__card-body">{e.body}</p>
          </div>
        </div>
      ))}
    </div>
  )
}
