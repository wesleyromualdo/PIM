<?php
namespace Simec\Par3;

/**
 * Especificação do controller do PAR3
 */
trait ControleTrait
{

    /**
     * Retorna a responsabilidade do usuário
     * @return \Par3\ResponsabilidadeDoUsuario
     */
    protected function usuarioResponsabilidade()
    {
        return new UsuarioResponsabilidade();
    }

    /**
     * Retorna o usuário logado na sessão
     * @return \Simec\Par3\UsuarioSessao
     */
    protected function usuario()
    {
        return new UsuarioSessao();
    }
}
