<?php

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

class Fichamonitoramentoestado extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'sase.fichamonitoramentoestado';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'fmeid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'fmedataalteracao' => null,
        'fmedatainclusao' => null,
        'usucpf' => null,
        'fmestatus' => null,
        'fmeequipeatolegal' => null,
        'fmeequipecontato' => null,
        'fmeequipetelefone' => null,
        'fmeequipeemail' => null,
        'fmeequipetecnica' => null,
        'fmecomnumanoatolegal' => null,
        'fmecomcoordenadora' => null,
        'fmeperavalano4' => null,
        'fmeperavalano3' => null,
        'fmeperavalano2' => null,
        'fmeperavalano1' => null,
        'fmeperavalquadrienal' => null,
        'fmeperavalquinquenal' => null,
        'fmeperavalnaoprevisto' => null,
        'fmeperavaltrianual' => null,
        'fmeperavalbianual' => null,
        'fmeperavalanual' => null,
        'fmepme' => null,
        'estuf' => null,
        'fmeid' => null,
    );
    
    protected $arAtributosInt = array(
            'fmeid' => null,
            'fmeperavalano1' => null,
            'fmeperavalano2' => null,
            'fmeperavalano3' => null,
            'fmeperavalano4' => null
    );
    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'fmedataalteracao' => array('allowEmpty' => true),
            'fmedatainclusao' => array('allowEmpty' => true),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'fmestatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'fmeequipeatolegal' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 30))),
            'fmeequipecontato' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
            'fmeequipetelefone' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'fmeequipeemail' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'fmeequipetecnica' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'fmecomnumanoatolegal' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 30))),
            'fmecomcoordenadora' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'fmeperavalano4' => array('allowEmpty' => true),
            'fmeperavalano3' => array('allowEmpty' => true),
            'fmeperavalano2' => array('allowEmpty' => true),
            'fmeperavalano1' => array('allowEmpty' => true),
            'fmeperavalquadrienal' => array('allowEmpty' => true),
            'fmeperavalquinquenal' => array('allowEmpty' => true),
            'fmeperavalnaoprevisto' => array('allowEmpty' => true),
            'fmeperavaltrianual' => array('allowEmpty' => true),
            'fmeperavalbianual' => array('allowEmpty' => true),
            'fmeperavalanual' => array('allowEmpty' => true),
            'fmepme' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 30))),
            'estuf' => array(new Zend_Validate_StringLength(array('max' => 2))),
            'fmeid' => array('Digits'),
        );
    }
    
    /**
     * Monta query para a função monta_lista da classe_simec.
     *
     * @param array $filtros
     * @author Sávio Resende
     * @return $sql
     */
    public function montaListaQuery( $filtros = false, $filtroSemPadrao = false, $order = false ){

        $acao  = "'<a style=\"cursor:pointer\" onclick=\"editarMonitoramentoEstado( ' || a.fmeid || ' )\"><img src=\"/imagens/alterar.gif\" title=\"Editar\"/></a>'";

        $indVerde = <<<HTML
    '<img title="Preenchido" src="/imagens/pnld/p_verde.gif"/>'
HTML;
        $indVermelho = <<<HTML
    '<img title="Não preenchido" src="/imagens/pnld/p_vermelho.gif"/>'
HTML;

        $sql = "
                SELECT
                    {$acao} as acao,
                    e.estdescricao as estado,
                    CASE
                      WHEN
                            usucpf is not null 
                        AND fmedatainclusao is not null 
                        AND fmepme is not null
                        AND (fmeperavalanual = 't' OR fmeperavalbianual = 't' OR fmeperavaltrianual = 't' OR fmeperavalquadrienal = 't' OR fmeperavalquinquenal = 't' OR fmeperavalnaoprevisto = 't' )
                        AND fmeperavalano1 is not null
                        AND fmecomcoordenadora is not null
                        AND fmecomnumanoatolegal is not null
                        AND fmeequipetecnica is not null
                        AND fmeequipeatolegal is not null
                        AND (fmeequipetelefone is not null OR fmeequipeemail is not null)
                      THEN {$indVerde}
                      ELSE {$indVermelho}
                    END as preenchido
                FROM " . $this->stNomeTabela . " a
                JOIN territorios.estado e ON e.estuf = a.estuf";
        $sql .= " WHERE 1=1 AND fmestatus = 'A'";
       // $sql .= " ORDER BY a.estuf, a.muncod ASC";


        if( $filtros != false ){
            foreach ($filtros as $chave => $valor)
                $sql .= " AND " . $chave . " = " . ((!in_array($chave, $this->arAtributosInt))?"'".$valor."'":$valor) . " ";
        }
        if( $filtroSemPadrao != false ){
            $sql .= " AND ";
            foreach ($filtroSemPadrao as $chave => $string)
                $sql .= " " . $string . " ";
        }
        if( $order != false){
            $sql.= " ORDER BY "."$order";
        }
        return $sql;
    }
/**
     * Tratamento de regras de perfis para geração de filtros para 'montaListaQuery'
     *
     */
    public function trataRegrasPerfis( $usucpf, $sql ){

            $perfis = pegaPerfilGeral( $usucpf );

            $sql_temp = "   select 
			ur.pflcod AS pflcod,
			te.estuf AS estuf,
			ur.muncod AS muncod 
		 from 
                 sase.usuarioresponsabilidade ur
                 Inner join territorios.municipio tm ON  tm.muncod = ur.muncod OR tm.estuf = ur.estuf
                 inner join territorios.estado te ON te.estuf = tm.estuf
                 where ur.usucpf = '{$usucpf}' AND ur.rpustatus = 'A'; ";
            $responsabilidades = $this->carregar( $sql_temp );

            // resgata estados
            $estados = array();
            if( !empty($responsabilidades) )
            foreach ($responsabilidades as $key => $value) {

                    if( !empty($value['estuf']) )
                            array_push($estados, $value['estuf']);
            }

            $OR = false;

            if( in_array( PFLCOD_SASE_TECNICO, $perfis) || in_array( PFLCOD_SASE_EXECUTIVO,$perfis) || in_array( PFLCOD_SASE_SUPERVISOR_GERAL, $perfis) ){
                    $sql .= " AND ( a.estuf in ('".(implode('\',\'',$estados))."') )";
                    $OR = true;
            }

            return $sql;
    }
    
    public function montaListaXls(  $filtros = false, $filtroSemPadrao = false, $order = false ){
        global $db;

        $sql = "
            SELECT
                e.estdescricao as estado,
                CASE
                  when usucpf is not null 
                        AND fmedatainclusao is not null 
                        AND fmepme is not null
                        AND (fmeperavalanual = 't' OR fmeperavalbianual = 't' OR fmeperavaltrianual = 't' OR fmeperavalquadrienal = 't' OR fmeperavalquinquenal = 't' OR fmeperavalnaoprevisto = 't' )
                        AND fmeperavalano1 is not null
                        AND fmecomcoordenadora is not null
                        AND fmecomnumanoatolegal is not null
                        AND fmeequipetecnica is not null
                        AND fmeequipeatolegal is not null
                        AND (fmeequipetelefone is not null OR fmeequipeemail is not null)
                  then 'Preenchido'
                  else 'Não preenchido'
                END as preenchido
            FROM " . $this->stNomeTabela . " a
            JOIN territorios.estado e ON e.estuf = a.estuf";
        $sql .= " WHERE 1=1 AND fmestatus = 'A'";

        if( $filtros != false ){
            foreach ($filtros as $chave => $valor)
            $sql .= " AND " . $chave . " = " . ((!in_array($chave, $this->arAtributosInt))?"'".$valor."'":$valor) . " ";
        }
        if( $filtroSemPadrao != false ){
            $sql .= " AND ";
            foreach ($filtroSemPadrao as $chave => $string)
                $sql .= " " . $string . " ";
        }
        if( $order != false){
            $sql.= " ORDER BY "."$order";
        }

        $cabecalho = array("Estado","Situação");
        $alinhamento = array('left','left','left', "left",'left','left');
        $larguras = array('5%','5%','75%', '10%', '5%', '5%');

        ob_clean();
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=simec_sase_assistenciatecnicaestado_" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=simec_sase_assistenciatecnicaestado_" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");

        $db->monta_lista($sql,$cabecalho,100000,5,'N','','N','listaAssessoramento',$larguras,$alinhamento);
        //$db->monta_lista_tabulado($sql, $cabecalho, 100000, 5, 'N', '100%', '');
    }

    /**
     * Exclui Logicamente Monitoramento
     *
     * @param integer $fmeid
     * @return bool
     * @author Sávio Resende
     */
    public function excluirLogicamenteMonitoramento( $fmeid ){
            $this->carregarPorId( $fmeid );
            $this->arAtributos['fmestatus'] = 'I';
            $this->alterar();
            return $this->commit();
    }
        
     /**
     * Verifica se já existe ficha cadastrada, para inativar caso exista.
     *
     * @param object $dadosMonitoramento
     * @author 
     */
    public function salvarHistorico( $dadosMonitoramento ){

        if( !isset($dadosMonitoramento->estuf) ) return false;
        $existe = $this->pegaUm( " SELECT COUNT(*) as total FROM " . $this->stNomeTabela . " WHERE estuf = '" . $dadosMonitoramento->estuf . "' AND usucpf is not null" );

        if( $existe ){
            $sql = " UPDATE $this->stNomeTabela SET fmestatus = 'I' WHERE estuf = '" . $dadosMonitoramento->estuf . "' ";
            $resultado = $this->executar( $sql );

            if( $resultado ) return $resultado;
        }
            return false;
    }
    
    /**
    * Monta legenda de acordo com os estados selecionados
    *
    * @param array $estuf
    * @return html
    */
    public function montaLegendaEstado( Array $estuf ){
        global $db;?>
        <div id="legendaMapaContainer">
        <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
            <ul>
                <?php
                require_once dirname(__FILE__) . '/Mapa/Metadados/DadoFichaMonitoramentoEstadoExterno.php';
                $classeDados = 'DadoFichaMonitoramentoEstadoExterno';
                $legenda = (new $classeDados())->dadosDaLegenda();
                foreach ($legenda as $value) {
                    echo <<<HTML

                    <li >
                           <table>
                              <tr>
                                 <td>
                                    <span style='background:{$value['cor']};' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;
                                        <b>{$value['total']}</b>&nbsp;&nbsp;
                                 </td>
                                 <td>
                                    {$value['descricao']}
                                 </td>
                              </tr>
                           </table>
                          </li>
HTML;
                }
            ?>
            </ul>
        </div><?php
    }
    public function dadosHistorico($estado){
        global $db;
        $sql = "SELECT fme.estuf,
                        fmepme,
                        CASE 
                            WHEN fmeperavalanual = TRUE THEN 'Anual'
                            WHEN fmeperavalbianual = TRUE THEN 'Bianual'
                            WHEN fmeperavaltrianual = TRUE THEN 'Trianual'
                            WHEN fmeperavalquadrienal = TRUE THEN 'Quadrienal'
                            WHEN fmeperavalquinquenal = TRUE THEN 'Quinquenal'
                            WHEN fmeperavalnaoprevisto = TRUE THEN 'Não previsto'
                            ELSE '<center> -- </center>'
                        END as periodo,
                        CASE 
                            WHEN fmeperavalano1 IS NOT NULL THEN fmeperavalano1::varchar
                            ELSE '<center> -- </center>'
                        END as ano,
                        fmecomcoordenadora,
                        fmecomnumanoatolegal,
                        fmeequipetecnica,
                        fmeequipeatolegal,
                        CASE WHEN fmestatus = 'I' THEN 'Inativo' ELSE 'Ativo' END fmestatus,
                        fme.usucpf ,
                        usunome,
                        fmedatainclusao,
                        fmedataalteracao 
                 FROM sase.fichamonitoramentoestado fme
                 JOIN seguranca.usuario usu ON usu.usucpf = fme.usucpf
                 WHERE fme.estuf = '{$estado}' AND fmestatus = 'I'
                 ORDER BY fmedatainclusao,fmedataalteracao, fme.usucpf ASC";

        $cabecalho = array("Estado","Plano Municipal de Educação","Período","Ano(s)","Comissão", "Ato (n° e ano)", "Equipe", "Ato Legal","Status", "CPF","Nome","Inclusão","Alteração");
        $alinhamento = array('left','left','left','left','left','left','left','left','left');
        //$larguras = array('5%','10%','15%','5%','10%','15%','5%','10%','5%','1%','5%','8%','4%','4%');
        $historico = $db->monta_lista($sql,$cabecalho,30,5,'N','','N','historico',$larguras,$alinhamento); 
        return $historico;
    }
    /**
     * Monta legenda de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situação
     *
     * @author Sávio Resende
     * @param array $estuf
     * @return html
     */
    public function montaLegendaEstadoExterno()
    {
        global $db; ?>
        <div id="legendaConteudo" style="display:block;float:right;width:285px;" class="topo_bootstrap_off">
            <div id="tituloLegenda">
                <div style="float:left;"><h5 style="font-size:14px !important;margin:2px !important;margin-left:0px !important;">&nbsp;Legenda:</h5></div>
                <div style="float:right;"></div>
                <div style="clear:both;height:1px;">&nbsp;</div>
            </div>
            <ul>
            <?php
                require_once dirname(__FILE__) . '/Mapa/Metadados/DadoFichaMonitoramentoEstadoExterno.php';
                $classeDados = 'DadoFichaMonitoramentoEstadoExterno';
                $legenda = (new $classeDados())->dadosDaLegenda();
                foreach ($legenda as $value) {
                    echo <<<HTML
                        <li>
                            <table>
                                <tr>
                                    <td style="width:47px;">
                                        <span style="background:{$value['cor']};" class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>{$value['total']}</b>&nbsp;&nbsp;
                                    </td>
                                    <td>{$value['descricao']}</td>
                                </tr>
                            </table>
                        </li>
HTML;
                }
            ?>
            </ul>
        </div>
    <?php
    }

}
