<?php

/**
 * Classe Territorio
 *
 * @author SÃ¡vio Resende <savio@savioresende.com.br>
 *
 * Objeto de Modelo de Territorio
 */
class Territorio extends Modelo
{

    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sase.territorio";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('terid');

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'terid' => null,
        'mesid' => null,
        'muncod' => null,
        'usucpf' => null,
        'terstatus' => null
    );

    /**
     * Atributos String da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosStr = array();

    /**
     * Campos da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arCampos = array(
        'terid',
        'mesid',
        'muncod',
        'usucpf',
        'terstatus'
    );

    /**
     * Campos ObrigatÃ³rios da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arAtributosObrigatorios = array(
        'mesid',
        'muncod',
        'terstatus'
    );
    /**
     * Atributo indireto estado
     */
    public $estuf = null;
    public $orgid = null;

    /**
     * Monta query para a funÃ§Ã£o monta_lista da classe_simec
     *
     * @param array $filtros
     * @author SÃ¡vio Resende
     * @return $sql
     */
    public function montaListaQuery( $filtros = false ){
        $sql = "
            SELECT          '<a style=\"cursor:pointer\" onclick=\"deletaOrganizacaoTerritorio( ' || inf.infterid || ' )\"><img src=\"/imagens/excluir.gif\"/></a>
                             <a style=\"cursor:pointer\" onclick=\"editarTerritorio( ' || inf.infterid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>' as acao,
			                o.orgdsc,
			                inf.infternomeorganizacao,
			                (select count(*) from sase.territoriomunicipio c where c.infterid = inf.infterid ) qnt,
			                CASE WHEN inf.infterstatus = 'I'
			                THEN 'Inativo'
			                WHEN inf.infterstatus = 'A'
			                THEN 'Ativo'
			                END AS infterstatus
            FROM    		sase.informacoesterritorio inf
            INNER JOIN 		sase.orgao o ON inf.orgid = o.orgid 
			WHERE 			o.orgstatus = 'A' 

			";
        if (!empty($filtros['o.orgid'])){
                $sql .= " AND o.orgid = {$filtros['o.orgid']}";
        }

        if (!empty($filtros['e.estuf'])){
            $sql .= " AND inf.infterid IN (
                   SELECT sase_tm.infterid
                   FROM  sase.territoriomunicipio sase_tm
                       INNER JOIN territorios.municipio tm ON tm.muncod = sase_tm.muncod
                       INNER JOIN territorios.estado te ON tm.estuf = te.estuf
                   WHERE sase_tm.terstatus = 'A' AND te.estuf IN ('{$filtros['e.estuf']}')
                 )";
        }

        if (!empty($filtros['t.muncod'])){
            $sql .= " AND inf.infterid IN (
                   SELECT sase_tm.infterid
                   FROM  sase.territoriomunicipio sase_tm
                       INNER JOIN territorios.municipio tm ON tm.muncod = sase_tm.muncod
                       INNER JOIN territorios.estado e ON tm.estuf = e.estuf
                   WHERE sase_tm.terstatus = 'A'
                   AND e.estuf in ('{$filtros['e.estuf']}')
                   AND sase_tm.muncod IN ('{$filtros['t.muncod']}')
                  ) 
                ";
        }

        if (!empty($filtros['t.terstatus'])){
            $sql .= " AND inf.infterstatus = '{$filtros['t.terstatus']}' ";
        }

        return $sql;
    }

    public function montaListaXls( $filtros = false ){
        global $db;

        $indLeiVerde = <<<HTML
'<img title="Com lei" src="/imagens/pnld/p_verde.gif"/>'
HTML;
        $indLeiVermelho = <<<HTML
'<img title="Sem lei" src="/imagens/pnld/p_vermelho.gif"/>'
HTML;
        $indAdesaoVerde = <<<HTML
'<img title="Com adesão" src="/imagens/pnld/p_verde.gif"/>'
HTML;
        $indAdesaoVermelho = <<<HTML
'<img title="Sem adesão" src="/imagens/pnld/p_vermelho.gif"/>'
HTML;

        $sql = "
			SELECT 
			DISTINCT 
                o.orgdsc,
                mes.mesdsc,
				COUNT(mes.mesid),
				CASE WHEN terstatus = 'I' THEN 'Inativo' WHEN terstatus = 'A' THEN 'Ativo' END AS terstatus
			FROM " . $this->stNomeTabela . " t
			JOIN territorios.municipio m ON m.muncod = t.muncod
			JOIN territorios.estado e ON e.estuf = m.estuf
			JOIN sase.mesoregiao mes ON mes.mesid = t.mesid
                        left join sase.orgaoestado oes on oes.oesid = mes.oesid
                        left join sase.orgao o on o.orgid = oes.orgid";
        if( $filtros != false ){
            $sql .= " WHERE 1=1 ";
            foreach ($filtros as $chave => $valor)
                $sql .= " AND " . $chave . ((in_array($chave, $this->arAtributosStr))?" LIKE '%".$valor."%'":" in ( '".$valor."')") . " ";
        }
        $sql .= " GROUP BY  e.estuf, o.orgdsc, mes.mesdsc, terstatus ";

        $cabecalho = array( "Tipo de Organização", "Nome Território", "Total de Municípios", "Status");
        $alinhamento = array( 'center', 'center', 'center', 'center' );
        $larguras = array( '25%', '25%', '25%', '25%');

        ob_clean();
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=simec_sase_organizacoes_territoriais_" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=simec_sase_organizacoes_territoriais_" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");

        $db->monta_lista($sql,$cabecalho,1000000,5,'N','','N','listaTerritorio',$larguras,$alinhamento);
    }

    /**
     * Atualiza Territorio
     *
     * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
     * @author SÃ¡vio Resende
     */
    public function atualizarTerritorio(){
        if( $this->validaCamposObrigatorios() ){
            $this->alterar();
            return $this->commit();
        }

        return 'invalido';
    }

    public function SalvarTerritorio(){
        $this->popularDadosObjeto();
        if( $this->validaCamposObrigatorios() ){
            $this->salvar();
            return $this->commit();
        }
        return false;
    }
    /**
     * Exclui logicamente o Territorio
     *
     * @param integer $stacod
     * @return bool
     * @author SÃ¡vio Resende
     */
    public function excluirTerritorio( $terid ){
        $this->carregarPorId( $terid );
        $this->arAtributos['terstatus'] = 'I';
        $this->alterar();
        return $this->commit();
    }

    /**
     * Popula Objeto com Array
     *
     * @param array $arDados
     * @return $this
     * @author SÃ¡vio Resende
     */
    public function populaTerritorio( Array $arDados ){
        $this->popularObjeto( $this->arCampos, $arDados );
        return $this;
    }

    /**
     * Valida campos obrigatorios no objeto populado
     *
     * @author SÃ¡vio Resende
     * @return bool
     */
    public function validaCamposObrigatorios(){
        foreach ($this->arAtributosObrigatorios as $chave => $valor)
            if( !isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]) )
                return false;

        return true;
    }

    /**
     * Cadastra Territorio Montado no Objeto
     *
     * @author SÃ¡vio Resende
     * @return bool|string - retorna string 'invalido' para quando tiver campos obrigatorios vazios
     */
    public function cadastrarTerritorio(){
        if( $this->validaCamposObrigatorios() ){
            $this->terid = $this->inserir();
            return $this->commit();
        }

        return 'invalido';
    }

    /**
     * Monta objeto TerritÃ³rio
     *
     * @param $terid
     * @author SÃ¡vio Resende
     */
    public function carregarTerritorio( $terid ){

        $this->carregarPorId( $terid );
        if( $this->muncod ){
            $sql = " select estuf from territorios.municipio where muncod = '" . $this->muncod . "' ";
            $this->estuf = $this->pegaUm( $sql );

            // carrega o Ã³rgao
            $sql = " select orgid from sase.territorio t 
					inner join sase.mesoregiao mes on mes.mesid = t.mesid
					inner join sase.orgaoestado oes on oes.oesid = mes.oesid
					where terid = '" . $terid . "' ";

            $this->orgid = $this->pegaUm( $sql );
        }

    }

    /**
     * Monta legenda de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situaÃ§Ã£o
     *
     * @author SÃ¡vio Resende
     * @param array $estuf
     * @return html
     */
    public function montaLegenda( Array $estuf, $orgid ){
        global $db;
        ?>
        <div id="legendaMapaContainer">
            <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
            <!--		<ul>-->
            <?php
            foreach($estuf as $uf){
                if($uf == "DF"){
                    echo "<ul>";
                    echo "<li><tr><td><font color='#428bca' >A cor no mapa vincula-se ao município com o respectivo código IBGE. Por isso, embora o DF tenha 40 Coordenações Regionais de Ensino  (CREs), possui somente 1 código IBGE, portanto o mapa é representado com uma única cor.</font></td></tr></table></li>";
                    echo "</ul>";
                }
            }
            $sql = "
                        select
                                m.mesdsc as mesdsc, 
                                m.mescor as mescor, 
                                mun.estuf as estuf, 
                                o.orgdsc as orgdsc, 
                                oes.orgid as orgid,
                                m.mesid as mesid
                        from sase.mesoregiao m
                        left join sase.orgaoestado oes on oes.oesid = m.oesid
                        left join sase.orgao o on o.orgid = oes.orgid
                        left join sase.territorio t on t.mesid = m.mesid
                        join territorios.municipio mun on mun.muncod = t.muncod and mun.estuf = oes.estuf
                        where messtatus = 'A' and o.orgid = '{$orgid}'
                        and oes.estuf in ( '" . (implode("','", $estuf)) . "' )
                        group by 1,2,3,4,5,6
                        order by m.mesdsc ASC
                ";
            $lista = $db->carregar( $sql );
            if (is_array($lista)) {
                echo "<ul>";
                $est = "";
                $totalGeral = 0;
                echo "<li><table cellpadding=5><tr><td><b>Região</b></td><td style='text-align:right;'>Municípios</td></tr></li>";
                foreach ($lista as $key => $value) {
                    $sql = "
					select count(t.terid) as total
					from sase.territorio t
					join sase.mesoregiao mes on mes.mesid = t.mesid
					join territorios.municipio m on m.muncod = t.muncod
					where terstatus = 'A' and
						mes.mesid = {$value['mesid']} 
						" . (($estuf != '' && count($estuf) > 0) ? " and m.estuf in ( '" . (implode("','", $estuf)) . "' ) " : "");
                    $total = $db->pegaUm($sql);
                    $totalGeral += $total;
                    echo "<li><tr><td><span style='background:" . $value['mescor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;" . $value['mesdsc'] . "&nbsp;&nbsp;</td><td style='text-align:right;'><b>" . (int)$total . "</b></td></tr></li>";
                }
                echo "<li><tr><td>&nbsp;&nbsp;Total&nbsp;&nbsp;</td><td style='text-align:right;'><b>" . (int)$totalGeral . "</b></td></tr></table></li>";
                echo "</ul>";
            }
            ?>
            <!--		</ul>-->
        </div>
        <?php
    }

}
