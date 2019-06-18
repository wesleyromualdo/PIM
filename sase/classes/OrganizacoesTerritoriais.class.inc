<?php

/**
 * Classe Territorio
 *
 * @author SÃ¡vio Resende <savio@savioresende.com.br>
 *
 * Objeto de Modelo de Territorio
 */
class OrganizacoesTerritoriais extends Modelo
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

    public function getOptionEstado()
    {
        $sql = "SELECT estuf, estdescricao FROM territorios.estado ORDER BY estdescricao ASC ";
        $estados = $this->carregar($sql);
        foreach ($estados as $key => $value) {
            echo "<option value='" . $value['estuf'] . "'>" . $value['estdescricao'] . "</option>";
        }
    }

    public function carregarOrgaos()
    {
        if (empty($estuf))
            $estuf = $_POST['estuf'];
        if ($estuf != '') {
            $sql = "SELECT org.orgid, orgdsc FROM sase.orgao org INNER JOIN sase.orgaoestado oes ON oes.orgid = org.orgid WHERE oes.estuf = '{$estuf}' ORDER BY org.orgid ASC";
        } else {
            $sql = "SELECT orgid, orgdsc FROM sase.orgao ORDER BY orgid ASC";
        }
        $res = $this->carregar($sql);

        $html = <<<HTML
            <select id="orgao" name="orgao" onchange="javascript:filtrar();" class="multiselect">
HTML;
        $i = 1;
        if (is_array($res)) {
            foreach ($res as $key => $value) {
                $attr = $i == 1 ? 'selected' : '';
                $html .= "<option value='" . $value['orgid'] . "' $attr >" . $value['orgdsc'] . "</option>";
                $i++;
            }
        }

        $html .= <<<HTML
            </select>
HTML;

        echo $html;
    }

    private function getMunicipioPorEstado($uf)
    {
        $where = ' WHERE 1 = 1';
        if (is_array($uf)) {
            $strUf = implode("','", $uf);
            $where .= " AND estuf IN ('{$strUf}') ";
        }
        $sql = " SELECT muncod, mundescricao FROM territorios.municipio {$where} ORDER BY mundescricao ASC ";
        $municipios = $this->carregar($sql);
        $municipios = is_array($municipios) ? $municipios : array();
        return $municipios;

    }

    public function getOptionMunicipios($uf)
    {
        $municipios = $this->getMunicipioPorEstado($uf);
        $option = '<option value="">Selecione um Estado</option>';
        foreach ($municipios as $chave => $municipio) {
            $option .= " <option value='{$municipio['muncod']}'>{$municipio['mundescricao']}</option>";
        }
        echo $option;
    }

    public function montaLegenda(Array $estuf, $orgid, $muncod)
    {
        global $db;
        ?>
        <div id="legendaMapaContainer" style="width: 100%;margin-left: 0px">
            <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
            <?php
            foreach ($estuf as $uf) {
                if ($uf == "DF") {
                    echo <<<HTML
                        <table>
                            <tr>
                                <td><font color='#428bca' >
                                A cor no mapa vincula-se ao município com o respectivo código IBGE. Por isso, embora o DF tenha 40 Coordenações Regionais de Ensino  
                                (CREs), possui somente 1 código IBGE, portanto o mapa é representado com uma única cor.</font>
                                </td>
                            </tr>
                        </table>
HTML;
                }
            }

            $where = "";
            if($muncod){
                $where .= " AND mun.muncod = '{$muncod}' ";
            }
            if($uf){
                $ufs = (implode("','", $estuf));
                $where .= " and oes.estuf in ( '{$ufs}' ) ";
            }

            $sql = <<<SQL
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
                        {$where}
                        group by 1,2,3,4,5,6
                        order by m.mesdsc ASC
SQL;
            $lista = $this->carregar($sql);


            if (is_array($lista)) {
                $totalGeral = 0;
                $totalTerritorio = count($lista);

                echo <<<HTML
                    <br>
                    <table cellpadding=5 style="font-size: 9px; line-height: 13px;width: 100%" border="1">
                        <tr>
                            <td style="text-align: center"><b>Território</b></td>
                            <td><b>Municípios</b></td>
                         </tr>
HTML;

                foreach ($lista as $key => $value) {
                    $sql = "
					SELECT count(t.terid) as total
					FROM sase.territorio t
					JOIN sase.mesoregiao mes on mes.mesid = t.mesid
					JOIN territorios.municipio m on m.muncod = t.muncod
					WHERE terstatus = 'A' 
						and mes.mesid = {$value['mesid']} 
						    " . (($estuf != '' && count($estuf) > 0) ? " and m.estuf in ( '" . (implode("','", $estuf)) . "' ) " : "");

                    $total = $db->pegaUm($sql);
                    $totalGeral += $total;
                    echo <<<HTML
                        <tr>
                            <td onclick="Mapas.buscaMesoForm({$value['mesid']},'{$value['estuf']}')"><span style='background:{$value['mescor']};' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp; {$value['mesdsc']}</td>
                            <td style='text-align:center;'><b> {$total} </b></td>
                        </tr>
HTML;
                }

                echo <<<HTML
                        <tr>
                            <td style='text-align:right;'><b>Total de Municípios</b></td>
                            <td style='text-align:center;'><b>{$totalGeral}</b></td>
                         </tr>
                         <tr>
                            <td style='text-align:right;'><b>Total de Territórios</b></td>
                            <td style='text-align:center;'><b>{$totalTerritorio}</b></td>
                         </tr>
                     </table>
HTML;
            }
            ?>
        </div>
        <?php
    }

}
