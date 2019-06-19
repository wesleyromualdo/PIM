<?php
/**
 * $Id: Comment.php 104958 2015-11-18 19:22:29Z maykelbraz $
 */

/**
 * Ação de comentário da Listagem.
 * Serve também para adicionar, quanto para listar comentários.
 */
class Simec_Listagem_Acao_Comment extends Simec_Listagem_AcaoComID
{
    protected $icone = 'comment';
    protected $titulo = 'Comentários';
    protected $cor = 'info';
}