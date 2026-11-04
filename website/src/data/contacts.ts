import githubIcon from '../assets/github-white-icon.svg'
import linkedinIcon from '../assets/linkedin-app-white-icon.svg'
import huggingfaceIcon from '../assets/huggingface.svg'
import mailIcon from '../assets/mail.svg'

const EMAIL = 'saneaven@gmail.com'

interface Contact {
  label: string
  href: string
  icon: string
  external?: boolean
}

export const CONTACTS: Contact[] = [
  { label: 'GitHub', href: 'https://github.com/saneaven', icon: githubIcon, external: true },
  { label: 'LinkedIn', href: 'https://www.linkedin.com/in/saneaven', icon: linkedinIcon, external: true },
  { label: 'HuggingFace', href: 'https://huggingface.co/saneaven', icon: huggingfaceIcon, external: true },
  { label: 'Email', href: `mailto:${EMAIL}`, icon: mailIcon },
]
