import * as React from 'react'
import * as S from './style'

interface Props {
  on: boolean
  onClick: () => void
}

function Toggle (props: Props) {
  return (
    <S.Box
      type='button'
      role='switch'
      aria-checked={props.on}
      onClick={props.onClick}
      isActive={props.on}
    >
      <S.Circle isActive={props.on} />
    </S.Box>
  )
}

export default Toggle
