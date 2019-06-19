<?php
namespace Simec\Corporativo\Workflow\Controle;

use Simec\Corporativo\Workflow\Modelo\Documento as Documento;
use Simec\Corporativo\Workflow\Modelo\AcaoEstadoDocumento as AcaoEstadoDocumento;
use Simec\Corporativo\Workflow\Modelo\EstadoDocumentoPerfil as EstadoDocumentoPerfil;
use Simec\Corporativo\Workflow\Modelo\ComentarioDocumento as ComentarioDocumento;
use Simec\Corporativo\Workflow\Modelo\HistoricoDocumento as HistoricoDocumento;

use Simec\Corporativo\Workflow\Dado\HistoricoDocumento as DadoHistoricoDocumento;
use Simec\Corporativo\Workflow\Dado\ComentarioDocumento as DadoComentarioDocumento;
use Simec\Corporativo\Workflow\Dado\Documento as DadoDocumento;

trait TraitWorkflow
{
    private static $conjuntoDadoWorkflow;
    private static $templateUnicaVez;
    private $dadoDocumento;
//     private $dadoDocumentoIdentificacaoJS;
    public $WF_erro = ['chamada_funcao_interna' => [], 'acao_tramite_inexistente' => []];
//     static private $cont;
    
    final protected function initTraitServicosWorkflow()
    {
        if ($this->get('tramitarDocumentoWorkflow')) {
            $this->tratarTramitacaoDocumento();
        } elseif ($this->get('historicoDocumentoWorkflow')) {
            $this->verHistoricoDocumento();
        }
    }
    
    private function __iniciarWorkflow(
        $docid, 
        Array $parametroAdicionalParaFuncaoInterna, 
        $tituloAlternativoCaixa='', 
        $sobrescreverDocumento=false
    )
    {
        if (self::$conjuntoDadoWorkflow[$docid] && $sobrescreverDocumento == false) {
            $this->dadoDocumento = self::$conjuntoDadoWorkflow[$docid];
            return true;
        }
        
        $this->dadoDocumento = array();
        $this->dadoDocumento['parametro_adicional_para_funcao_interna'] = $parametroAdicionalParaFuncaoInterna;
        $this->dadoDocumento['titulo_caixa_workflow'] = ($tituloAlternativoCaixa ? $tituloAlternativoCaixa : 'Workflow');
        $this->dadoDocumento['testa_superuser'] = $_SESSION['testa_superuser'];
        $this->pegarDadosDocumento($docid);
        $this->pegarAcaoEstadoPermitidoPerfil();

        
        self::$conjuntoDadoWorkflow[$docid] = $this->dadoDocumento;
    }
    
    final public function WF_DesenharBarra(
        $docid, 
        Array $parametroAdicionalParaFuncaoInterna = array(), 
        $tituloAlternativoCaixa = '',
        $sobrescreverDocumento = false
    )
    {
        $this->__iniciarWorkflow($docid, $parametroAdicionalParaFuncaoInterna, $tituloAlternativoCaixa, $sobrescreverDocumento);
        
        $this->toView('dadoDocumento', $this->dadoDocumento);
        $this->toView('isAjax', $this->isAjax());
        $this->toView('templateUnicaVez', self::$templateUnicaVez);
        
        self::$templateUnicaVez = (self::$templateUnicaVez ? self::$templateUnicaVez : true);
        
        $this->toJs('workflowSimecDocumento', self::$conjuntoDadoWorkflow);
        
        $view = (count($this->dadoDocumento['docid']) ? 'WF_DesenhaBarra' : 'WF_DesenhaBarraDocumentoInexistente');
        return $this->getPartial('src/Simec/Corporativo/Workflow/Visao/' . $view);
    }
    
    final static public function Wf_formatarLocalizacao($unicod, $dados)
    {
        return <<<HTML
<abbr title="{$dados['unidsc']}" data-toggle="popover">{$unicod}</abbr>
HTML;
    }
    
    final static public function Wf_formatarEstadoDocumento($esddsc, $dados, $hstid)
    {
        $esddsc = self::Wf_alinhaParaEsquerda($esddsc);
        $dados['cmddsc'] = simec_htmlspecialchars(($dados['cmddsc']));
        
        return <<<HTML
<span id="hstid_{$hstid}" data-comentario="{$dados['cmddsc']}">{$esddsc}</span>
HTML;
    }
    
    /**
     * Alinha o texto para a esquerda
     * @param mixed $valor Valor para ser formatado.
     * @return String
     */
    final static public function Wf_alinhaParaEsquerda($valor) {
        $valor = "<p style=\"text-align: left !important;\">$valor</p>";
        return $valor;
    }
    
    private function verHistoricoDocumento()
    {
        $docid = $this->get('docid');
        if ($docid) {
            $this->toView('classe', get_class($this));
            /**************
             * Dados da listagem - início
             **************/
            $cabecalho = array(
                'Onde Estava',
                'O que aconteceu',
                'Quem fez',
                'Quando fez',
                'Localização',
                'Tempo no estado'
            );
            
            $modelHistorico = new HistoricoDocumento();
            $sql = $modelHistorico->listarSql($docid);
            
            $this->toView('cabecalho', $cabecalho);
            $this->toView('sql', $sql);
            /**************
             * Dados da listagem - fim
             **************/
            
            $this->showHtml('src/Simec/Corporativo/Workflow/Visao/verHistoricoDocumento');
        }
    }
    
    private function inserirHistorico($docid, $aedid)
    {
        $modelHistorico = new HistoricoDocumento([
            "aedid"   => $aedid,
            "docid"   => $docid,
            "usucpf"  => $_SESSION['usucpf'],
            "htddata" => date('Y-m-d')
        ]);
        $modelHistorico->incluir();
        
        return $modelHistorico;
    }
    
    private function inserirComentario($docid, $hstid, $comentario)
    {
        $dadoComentario  = new DadoComentarioDocumento();
        $dadoComentario->carregar([
            "docid"     => $docid,
            "hstid"     => $dadoHistorico->hstid,
            "cmddsc"    => addslashes($comentario),
            "cmddata"   => date('Y-m-d'),
            "cmdstatus" => 'A'
        ]);
        $modelComentario = new ComentarioDocumento();
        $modelComentario->incluir($dadoComentario);
        
        return $modelComentario;
    }
    
    private function atualizarEstadoDocumento($docid, $esdiddestino)
    {
        $modelDocumento = new Documento([
            "docid" => $docid,
            "esdid" => $esdiddestino
        ]);
        $modelDocumento->atualizar();
        
        return $modelDocumento;
    }
    
    private function tramitarDocumento(
        $docid, 
        $aedid, 
        $comentario = null, 
        Array $parametroAdicionalParaFuncaoInterna = array()
    )
    {
        $this->__iniciarWorkflow($docid, $parametroAdicionalParaFuncaoInterna, '', true);
        
        foreach ($this->dadoDocumento['todas_acoes_permitidas_perfil'] as $acao) {
            if ($acao['aedid'] != $aedid) {
                continue;
            }
            
            if ($acao['resultado_validacao_condicao'] !== true) {
                return false;   
            }
            
            $modelHistorico = $this->inserirHistorico($docid, $aedid);
            
            if ($acao['esdsncomentario'] == 't' && !$comentario) {
                return false;                    
            } elseif ($acao['esdsncomentario'] == 't') {
                $this->inserirComentario($docid, $modelHistorico->hstid, $comentario);
            }

            $this->atualizarEstadoDocumento($docid, $acao['esdiddestino']);
            $this->executarPosAcao($acao['aedposacao']);

            $modelHistorico->commit();
            
            return true;
        }
        
        $this->WF_erro['acao_tramite_inexistente'] = 'Este ID de ação do documento: '.$aedid.'. Não está disponível para este documento no estado atual dele.';
        
        return false;
    }
    
    private function executarPosAcao($funcaoPosAcao)
    {
        $funcaoPosAcao = trim($funcaoPosAcao);
        if ($funcaoPosAcao) {
            $arFuncaoPosAcaoTratada = $this->tratarChamadaFuncaoInterna($funcaoPosAcao);
            
            foreach ($arFuncaoPosAcaoTratada as $funcaoPosAcao) {
                if ($funcaoPosAcao['funcao']) {
                    call_user_func_array($funcaoPosAcao['funcao'], $funcaoPosAcao['parametro']);
                }
            }
        }
    }
    
    private function tratarTramitacaoDocumento()
    {
        if ($this->get('docid') && $this->get('aedid')) {
//             $this->__iniciarWorkflow($this->get('docid'), $this->get('parametro_adicional_para_funcao_interna'));
            
            if ($this->tramitarDocumento(
                $this->get('docid'), 
                $this->get('aedid'), 
                $this->get('comentario'), 
                $this->get('parametro_adicional_para_funcao_interna')
            )) {
                $html_barra = $this->WF_DesenharBarra(
                    $this->get('docid'),
                    $this->get('parametro_adicional_para_funcao_interna'),
                    $this->get('titulo_caixa_workflow'),
                    true
                );
                
                $this->toView('html_barra', $html_barra);
                $this->toView('sucesso_tramite', 1);
                $this->toView('js_documento', simec_json_encode($this->dadoDocumento));
            } else {
                $this->toView('html_barra', '');
                $this->toView('sucesso_tramite', 0);
                $this->toView('js_documento', '');                
            }
        }
        
        $this->showPartial('src/Simec/Corporativo/Workflow/Visao/tratarTramitacaoDocumento');
        die;
    }
    
    private function pegarDadosDocumento($docid)
    {
        $modelDocumento = new Documento();
        $arDocumento = $modelDocumento->pegarDados($docid);
        
        $this->dadoDocumento = array_merge($this->dadoDocumento, $arDocumento);
    }
    
    private function pegarAcaoEstado()
    {
        $modelAcaoEstadoDocumento = new AcaoEstadoDocumento();
        $this->dadoDocumento['todas_acoes'] = $modelAcaoEstadoDocumento->pegarAcaoPorEstado($this->dadoDocumento['esdid']);
    }
    
    private function pegarPerfilAssociadoAcao()
    {
        if ($this->dadoDocumento['testa_superuser']) {
            $this->dadoDocumento['perfil_por_acao'] = array();
            return;
        }
        
        $arAedid = array();
        foreach ($this->dadoDocumento['todas_acoes'] as $acao) {
            $arAedid[] = $acao['aedid'];            
        }
        
        $modelEstadoDocumentoPerfil = new EstadoDocumentoPerfil();
        $this->dadoDocumento['perfil_por_acao'] = $modelEstadoDocumentoPerfil->pegarPerfilPorAcao($arAedid);
    }
    
    private function pegarPerfilUsuario()
    {
        if ($this->dadoDocumento['testa_superuser']) {
            $this->dadoDocumento['perfil_usuario'] = array();
            return;
        }
        
        $arPerfil = ($_SESSION['perfils'][$_SESSION['sisid']] 
                        ? $_SESSION['perfilusuario'][$_SESSION['usucpf']][$_SESSION['sisid']] 
                        : array());
        
        $this->dadoDocumento['perfil_usuario'] = array();
        foreach ($arPerfil as $perfil) {
            $this->dadoDocumento['perfil_usuario'][] = $perfil['pflcod'];
        }
    }
    
    private function pegarAcaoEstadoPermitidoPerfil()
    {
        $this->pegarAcaoEstado();
        $this->pegarPerfilAssociadoAcao();
        $this->pegarPerfilUsuario();
        
        $this->dadoDocumento['todas_acoes_permitidas_perfil'] = array();
        foreach ($this->dadoDocumento['todas_acoes'] as $acao) {
            if (
                in_array($this->dadoDocumento['perfil_por_acao'][$acao['aedid']], $this->dadoDocumento['perfil_usuario']) ||
                $this->dadoDocumento['testa_superuser']
            ) {
                $acao['aedicone'] = $this->trocaIconeBootstrap($acao['aedicone']);
                $this->dadoDocumento['todas_acoes_permitidas_perfil'][] = $this->validarAcaoEstado($acao);
            }
        }
    }
    
    private function validarAcaoEstado($acao)
    {
        $bloqueio_periodo	    = trim($acao['bloqueio_periodo']);
        
        if ($bloqueio_periodo == 'S') {
            $funcaoInternaCondicao  = trim($acao['aedcondicao']);
            $arFuncaoInternaTratada = $this->tratarChamadaFuncaoInterna($funcaoInternaCondicao);
            
            if ($arFuncaoInternaTratada) {
                foreach ($arFuncaoInternaTratada as $funcaoInternaTratada) {
                    $validacao = call_user_func_array($funcaoInternaTratada['funcao'], $funcaoInternaTratada['parametro']);
                    
                    if ($validacao !== true) {
                        break;
                    }
                }
            } else {
                $validacao = true;
            }
        } else {
            $aeddatafim = trim($acao['aeddatafim']);
            
            $validacao = 'Esta ação foi desabilitada no sistema dia '.formata_data($aeddatafim).', para habilitá-la, é necessário entrar em contato com administrador do sistema.';
        }
        $acao['resultado_validacao_condicao'] = $validacao;
        
        return $acao;
    }
    
    private function tratarChamadaFuncaoInterna($funcaoInterna)
    {
        if (!$funcaoInterna) {
            return array();
        }
        
        $arFuncaoInterna = array_map("trim", explode(";", $funcaoInterna));
        
        $arFuncaoInternaTratada = array();
        foreach ($arFuncaoInterna as $funcao) {
            if (trim($funcao) == '') {
                continue;
            }
            
            $posAbre  = strpos($funcao, "(");
            $posFecha = strrpos($funcao, ")");
            
            $nomeFuncao = substr($funcao, 0, $posAbre);
            
            $arParametro = substr($funcao, ($posAbre + 1), (($posFecha - $posAbre) - 1));
            $arParametro = array_map("trim", explode(",", $arParametro));

            $arParametroTratado = array();
            foreach ($arParametro as $parametro) {
                $arParametroTratado[] = $this->dadoDocumento['parametro_adicional_para_funcao_interna'][$parametro];
            }
            
            $chamadaFuncao = ["funcao" => [$this, $nomeFuncao], "parametro" => $arParametroTratado];
            if (is_callable($chamadaFuncao['funcao'])) {
                $arFuncaoInternaTratada[] = $chamadaFuncao;
            } else {
                $this->WF_erro['chamada_funcao_interna'][] = ["funcao" => $nomeFuncao, "parametro" => $arParametroTratado];
            }
        }
        
        return $arFuncaoInternaTratada;
    }
    
    private function trocaIconeBootstrap($icone)
    {
        switch ($icone) {
            case '1.png':
                return array(' glyphicon-thumbs-up', "btn-success");
            case '2.png':
                return array(' glyphicon-thumbs-down', "btn-danger");
            case '3.png':
                return array(' glyphicon-share-alt', "btn-warning");
        }
    }
    
}
