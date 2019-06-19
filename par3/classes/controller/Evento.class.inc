<?php
/**
 * Classe de controle do Endereço
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-09-2015
 * @link     no link
 */

/**
 * Par3_Controller_Evento
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-09-2015
 * @link     no link
 */
class Par3_Controller_Evento
{
    const K_NIVEL_ACESSO_UNIDADE      = 'U';
    const K_NIVEL_ACESSO_CONFIGURACAO = 'C';

    /**
     * Função salvar
     * - retorna o evento da prefeitura.
     *
     * @return retorna um objeto evento prefeitura.
     *
     */
    public function salvar($dados)
    {
        $dados = simec_utf8_decode_recursive($dados);

        try{
            $modelEvento = new Par3_Model_Evento($dados['eveid']);
            $modelEventoTipo = new Par3_Model_EventoTipo($dados['evtid']);

            $start = $dados['hrInicio'] ? $dados['hrInicio'] : formata_data_sql($dados['evedatainicio']);

            $retorno = array('overlap' => true, 'start' => $start);

            if(isset($dados['eveid']) && $dados['evedatafim']){
                $retorno['end'] = formata_data_sql($dados['evedatafim']);
            }

            $modelEvento->evedsc = $dados['evedsc'] ? $dados['evedsc'] : $modelEventoTipo->evtdsc;

            $retorno['title'] = $modelEvento->evetitulo = $dados['evetitulo'] ? $dados['evetitulo'] : $modelEventoTipo->evtdsc;
            $retorno['color'] = $retorno['borderColor'] = $retorno['textColor'] = $modelEvento->evecor = $dados['evecor'] ? $dados['evecor'] : $modelEventoTipo->evtcor;
            $modelEvento->evedatainicio = $dados['evedatainicio'];
            $modelEvento->evedatafim = $dados['evedatafim'];
            $modelEvento->evtid = $modelEventoTipo->evtid;
            $modelEvento->inuid = $dados['inuid'] ? $dados['inuid'] : null;
            $modelEvento->evetipo = $dados['inuid'] ? Par3_Model_Evento::K_TIPO_INSTRUMENTO_UNIDADE : Par3_Model_Evento::K_TIPO_GERAL;

            $modelEvento->salvar();
            $modelEvento->commit();

            if(!empty($dados['evetitulo']) && $modelEvento->eveid && !$dados['inuid']){
                $this->salvarLocalizacao($modelEvento, $dados);
            }

            $retorno['id'] = $modelEvento->eveid;
            return $retorno;
        } catch(Exception $e) {
            $modelEvento->rollback();
            return null;
        }
    }

    public function excluir($eveid)
    {
        $modelEvento = new Par3_Model_Evento($eveid);

        try{
            $modelEvento->excluir($eveid);
            return $modelEvento->commit();
        } catch(Exception $e) {
            $modelEvento->rollback();
            return null;
        }
    }

    public function salvarLocalizacao($modelEvento, $dados)
    {
        $modelEventoLocalizacao = new Par3_Model_EventoLocalizacao();

        $modelEventoLocalizacao->excluirVarios(" eveid = {$modelEvento->eveid} ");

        $modelEventoLocalizacao->eveid = $modelEvento->eveid;

        if (isset($dados['muncod']) && is_array($dados['muncod'])) {
            foreach ($dados['muncod'] as $muncod) {
                $modelEventoLocalizacao->evlid = null;
                $modelEventoLocalizacao->muncod = $muncod;
                $modelEventoLocalizacao->salvar();
            }
            $modelEvento->evetipo = Par3_Model_Evento::K_TIPO_MUNICIPAL;
        } elseif(isset($dados['estuf']) && is_array($dados['estuf'])) {
            foreach ($dados['estuf'] as $estuf) {
                $modelEventoLocalizacao->evlid = null;
                $modelEventoLocalizacao->estuf = $estuf;
                $modelEventoLocalizacao->salvar();
            }
            $modelEvento->evetipo = Par3_Model_Evento::K_TIPO_ESTADUAL;
        } elseif(isset($dados['regcod']) && is_array($dados['regcod'])) {
            foreach ($dados['regcod'] as $regcod) {
                $modelEventoLocalizacao->evlid = null;
                $modelEventoLocalizacao->regcod = $regcod;
                $modelEventoLocalizacao->salvar();
            }
            $modelEvento->evetipo = Par3_Model_Evento::K_TIPO_REGIONAL;
        } else {
            $modelEvento->evetipo = Par3_Model_Evento::K_TIPO_GERAL;
        }
        $modelEvento->salvar();
        $modelEventoLocalizacao->commit();
    }


    public function recuperarEventos($inuid)
    {
        $inuid = $inuid ? $inuid : 0;
        $modelEvento = new Par3_Model_Evento();

        $nivelAcesso = $this->recuperarNivelAcesso($inuid);

        $sql = "select  e.*,
                        case
                                when evetipo = 'I' then 1
                                else 0
                        end editable
                from par3.evento e
                         left join par3.instrumentounidade iu on iu.inuid = $inuid
                         left join par3.eventolocalizacao el on el.eveid = e.eveid
                         left join territorios.estado est on est.estuf = iu.estuf
                where evestatus = 'A'
                and (
                        (e.evetipo) = ('G')  -- GERAL
                        or (e.evetipo, e.inuid) = ('I', $inuid) -- POR INUID
                        or ((e.evetipo ='E'  )";
        if($inuid != 0) {
            $sql .= "    and  (iu.estuf =  el.estuf)";
        }
        $sql .=") -- ESTADUAL
                        or ((e.evetipo = 'M')";
        if($inuid != 0) {
            $sql .= "
            and (iu.muncod = el.muncod)
            ";}
        $sql .= ")  -- MUNICIPAL
             or ((e.evetipo = 'R')
               ";
        if($inuid != 0){
        $sql .="and (est.regcod::int = el.regcod)";
        }

        $sql .= ") -- REGIONAL
        ) order by e.evedatainicio";

        $eventos = $modelEvento->carregar($sql);

        $aEventos = array();
        foreach($eventos as $evento){
            $aEventos[] = array(
                'title' => $evento['evetitulo'],
                'description' => $evento['evedsc'],
                'start' => $evento['evedatainicio'],
                'end' => $evento['evedatafim'],
                'color' => $evento['evecor'],
                'borderColor' => $evento['evecor'],
                'textColor' => $evento['evecor'],
                'id' => $evento['eveid'],
                'editable' => ($evento['editable'] || $nivelAcesso == self::K_NIVEL_ACESSO_CONFIGURACAO) ? true : false,
            );
        }

        return simec_json_encode($aEventos);
    }

    public function montarFormulario($dados)
    {
        global $simec;

        include_once APPRAIZ . 'par3/modulos/principal/evento/formulario.php';
    }

    public function recuperarNivelAcesso($inuid)
    {
        $perfis = pegaPerfilGeral($_SESSION['usucpf']);

        if($inuid){
            $modelUnidade = new Par3_Model_InstrumentoUnidade();
            $possuiAcesso = (bool) $modelUnidade->testaPermissaoUnidade($_REQUEST['inuid']);

            if($possuiAcesso){
                return self::K_NIVEL_ACESSO_UNIDADE;
            }
        } elseif(!empty($perfis) && in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)) {
            return self::K_NIVEL_ACESSO_CONFIGURACAO;
        }

        simec_redirecionar('/par3/par3.php?modulo=inicio&acao=C', 'error');
    }

    public function verificarPermissaoEditar($dados)
    {
//        return 'Não é possível editar eventos pré-configurados!';
        return null;
    }

}
?>