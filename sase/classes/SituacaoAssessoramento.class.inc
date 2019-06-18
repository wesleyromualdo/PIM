<?php

/**
 * Classe Situação de Assessoramento
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 *
 * Objeto de Modelo de Situação de Assessoramento
 */
class SituacaoAssessoramento extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.situacaoassessoramento";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('stacod');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'stacod' => null,
		'stadsc' => null,
		'stacor' => null,
		'stastatus' => null,
		'esdid' => null
	);

	/**
	 * Atributos String da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosStr = array(
		'stadsc'
	);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
		'stacod',
		'stadsc',
		'stacor',
		'stastatus',
		'esdid'
	);

	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'stadsc',
		'stacor',
		'stastatus'
	);

	/**
	 * Monta query para a função monta_lista da classe_simec
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 * @return $sql
	 */
	public function montaListaQuery( $filtros = false ){
		$sql = "
			SELECT
				'<a style=\"cursor:pointer\" onclick=\"editarSituacaoAssessoramento( ' || stacod || ' )\"><img src=\"/imagens/alterar.gif\"/></a>
				<a style=\"cursor:pointer\" onclick=\"excluirSituacaoAssessoramento( ' || stacod || ' )\"><img src=\"/imagens/excluir.gif\"/></a>' as acao,
				stadsc,
				stacor,
				CASE WHEN stastatus = 'I' THEN 'Inativo' WHEN stastatus = 'A' THEN 'Ativo' END AS stastatus
			FROM " . $this->stNomeTabela . "
			WHERE stastatus = 'A' ";
		if( $filtros != false ){
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . ((in_array($chave, $this->arAtributosStr))?" LIKE '%".$valor."%'":" = '".$valor."'") . " ";
		}
		$sql .= "             ORDER BY stacod ASC "; // não retirar os espaços!

		return $sql;
	}

	/**
	 * Atualiza Situacao de Assessoramento
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizarSituacaoassessoramento(){
		if( $this->validaCamposObrigatorios() ){
			$this->alterar();
			return $this->commit();
		}

		return 'invalido';
	}

	/**
	 * Exclui logicamente a situacao de assessoramento
 	 *
	 * @param integer $stacod
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluirSituacaoAssessoramento( $stacod ){
		$this->carregarPorId( $stacod );
		$this->arAtributos['stastatus'] = 'I';
		$this->alterar();
		return $this->commit();
	}

	/**
	 * Popula Objeto com Array
	 *
	 * @param array $arDados
	 * @return $this
	 * @author Sávio Resende
	 */
	public function populaSituacaoAssessoramento( Array $arDados ){
		$this->popularObjeto( $this->arCampos, $arDados );
		return $this;
	}

	/**
	 * Valida campos obrigatorios no objeto populado
	 *
	 * @author Sávio Resende
	 * @return bool
	 */
	public function validaCamposObrigatorios(){
		foreach ($this->arAtributosObrigatorios as $chave => $valor)
			if( !isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]) )
				return false;

		return true;
	}

	/**
	 * Cadastra Situacao Assessoramento Montada no Objeto
	 *
	 * @author Sávio Resende
	 * @return bool|string - retorna string 'invalido' para quando tiver campos obrigatorios vazios
	 */
	public function cadastrarSituacaoAssessoramento(){
		if( $this->validaCamposObrigatorios() ){
			$this->stacod = $this->inserir();
			return $this->commit();
		}

		return 'invalido';
	}

	/**
	 * Monta legenda de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situação
	 *
	 * @author Sávio Resende
	 * @param array $estuf
	 * @return html
	 */
	public function montaLegenda( Array $estuf ){
            global $db;
            ?>
            <div id="legendaMapaContainer">
            <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
                <ul>
                        <?php
                            $sql = "
                                    SELECT
                                            s.stacod,
                                            s.stadsc,
                                            s.stacor,
                                            count(a.assid) as total
                                    FROM sase.situacaoassessoramento s
                                    LEFT JOIN sase.assessoramento a on a.stacod = s.stacod
                                    WHERE stastatus = 'A'
                                    GROUP BY 1,2,3
                                    ORDER BY stacod ASC ";
                            $lista = $db->carregar( $sql );
                            foreach ($lista as $key => $value) {

                                    $sql = "
                                            SELECT count(d.docid) AS total
                                            FROM workflow.documento d
                                            INNER JOIN sase.situacaoassessoramento s ON d.esdid = s.esdid AND s.stacod = " . $value['stacod'] . "
                                            INNER JOIN sase.assessoramento a ON a.docid = d.docid
                                            " . (($estuf!=''&&count($estuf)>0)?" WHERE a.estuf in ( '". (implode( "','", $estuf )) ."' ) ":"") . " ";
                                    $total = $db->pegaUm( $sql );

                                    echo "<li ><table><tr><td><span style='background:" . $value['stacor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . (($total!='')?$total:'0') . "</b>&nbsp;&nbsp;</td><td>" . $value['stadsc'] . "</td></tr></table></li>";
                            }
                        ?>
                </ul>
            </div>
            <?php
	}

	/**
	 * Monta legenda Lei PNE e Sem Lei PNE de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situação
	 *
	 * @author Sávio Resende
	 * @param array $estuf
	 * @return html
	 */
	public function montaLegendaLeiMunicipio( Array $estuf ){
            global $db; ?>

            <div id="legendaMapaContainer">
            <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
                <ul>
                    <?php
                        $sqlcomlei = "SELECT count(a.assid) AS total FROM sase.assessoramento a WHERE a.assleipne is not null
                                  " . ( ($estuf!=''&&count($estuf) > 0) ? " AND a.estuf in ( '". ( implode( "','", $estuf ) ) ."' ) ":"") . "";
                        $totalcomlei = $db->pegaUm( $sqlcomlei );

                        $sqlsemlei = "SELECT count(a.assid) AS total FROM sase.assessoramento a WHERE a.assleipne is null
                                  " . ( ($estuf!=''&&count($estuf) > 0) ? " AND a.estuf in ( '". ( implode( "','", $estuf ) ) ."' ) ":"") . "";
                        $totalsemlei = $db->pegaUm( $sqlsemlei );
                        echo "<li >
                               <table>
                                  <tr>
                                     <td>
                                        <span style='background:#006600;' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;
                                            <b>" . (($totalcomlei!='')?$totalcomlei:'0') . "</b>&nbsp;&nbsp;
                                     </td>
                                     <td>
                                        Com Lei PNE
                                     </td>
                                  </tr>
                                  <tr><td>&nbsp;</td></tr>
                                  <tr>
                                     <td>
                                        <span style='background:#FFFFFF;' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;
                                            <b>" . (($totalsemlei!='')?$totalsemlei:'0') . "</b>&nbsp;&nbsp;
                                     </td>
                                     <td>
                                        Sem Lei PNE
                                     </td>
                                  </tr>
                               </table>
                              </li>";
                    ?>
                </ul>
            </div><?php
	}

	/**
	 * Monta legenda Adesão e Sem Adesão de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situação
	 *
	 * @author Sávio Resende
	 * @param array $estuf
	 * @return html
	 */
	public function montaLegendaAdesaoMunicipio( Array $estuf ){
            global $db;?>
            <div id="legendaMapaContainer">
            <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
                <ul>
                    <?php
                        $sqlComAdesao = "SELECT count(a.assid) AS total FROM sase.assessoramento a WHERE a.assadesao is not null
                                  ".( ( $estuf!='' && count($estuf)>0 ) ? " AND a.estuf in ( '". ( implode( "','", $estuf ) ) ."' ) " : "" )." ";
                        $totalComAdesao = $db->pegaUm( $sqlComAdesao );

                        $sqlSemAdesao = "SELECT count(a.assid) AS total FROM sase.assessoramento a WHERE a.assadesao is null
                                  ".( ( $estuf != '' && count($estuf) > 0 ) ? " AND a.estuf in ( '". (implode( "','", $estuf )) ."' ) " : "" )." ";
                        $totalSemAdesao = $db->pegaUm( $sqlSemAdesao );

                        echo "<li >
                               <table>
                                  <tr>
                                     <td>
                                        <span style='background:#006600;' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;
                                            <b>" . ( ($totalComAdesao != '') ? $totalComAdesao : '0' ) . "</b>&nbsp;&nbsp;
                                     </td>
                                     <td>
                                        Com Adesão
                                     </td>
                                  </tr>
                                  <tr><td>&nbsp;</td></tr>
                                  <tr>
                                     <td>
                                        <span style='background:#FFFFFF;' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;
                                            <b>" . ( ($totalSemAdesao != '') ? $totalSemAdesao : '0' ) . "</b>&nbsp;&nbsp;
                                     </td>
                                     <td>
                                        Sem Adesão
                                     </td>
                                  </tr>
                               </table>
                              </li>";
                    ?>
                </ul>
            </div><?php
	}

    /**
     * Monta legenda de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situação
     *
     * @author Sávio Resende
     * @param array $estuf
     * @return html
     */
    public function montaLegendaMunicipioExterno($tipoinfo = Mapas::TIPO_ADEQUACAO)
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
                switch ($tipoinfo) {
                    case Mapas::TIPO_LEIPNE:
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadoSituacaoAssessoramentoLeipne.php';
                        $classeDados = 'DadoSituacaoAssessoramentoLeipne';
                        break;
                    case Mapas::TIPO_ADESAO:
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadoSituacaoAssessoramentoAdesao.php';
                        $classeDados = 'DadoSituacaoAssessoramentoAdesao';
                        break;
                    case Mapas::TIPO_ADEQUACAO:
                    default:
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadoSituacaoAssessoramento.php';
                        $classeDados = 'DadoSituacaoAssessoramento';
                }

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

	/**
	 * Monta legenda de acordo com os estados selecionados
	 *
	 * @author Victor Martins Machado
	 * @param array $estuf
	 * @return html
	 */
	public function montaLegendaEstado( Array $estuf ){
		global $db;
		?>
			<div id="legendaMapaContainer">
			<div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
				<ul>
					<?php
					$sql = "
						SELECT
							s.stacod,
							s.stadsc,
							s.stacor,
							count(a.aseid) as total
						FROM sase.situacaoassessoramento s
						LEFT JOIN sase.assessoramentoestado a on a.stacod = s.stacod
						WHERE stastatus = 'A'
						GROUP BY 1,2,3
						ORDER BY stacod ASC ";
					$lista = $db->carregar( $sql );
					foreach ($lista as $key => $value) {

						$sql = "
							SELECT count(d.docid) AS total
							FROM workflow.documento d
							INNER JOIN sase.situacaoassessoramento s ON d.esdid = s.esdid AND s.stacod = " . $value['stacod'] . "
							INNER JOIN sase.assessoramentoestado a ON a.docid = d.docid
							" . (($estuf!=''&&count($estuf)>0)?" WHERE a.estuf in ( '". (implode( "','", $estuf )) ."' ) ":"") . " ";
						$total = $db->pegaUm( $sql );

						echo "<li ><table><tr><td><span style='background:" . $value['stacor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . (($value['total']!='')?$value['total']:'0') . "</b>&nbsp;&nbsp;</td><td>" . $value['stadsc'] . "</td></tr></table></li>";
					}
					?>
				</ul>
			</div>
			<?php
		}
	/**
	 * Monta legenda de acordo com os estados selecionados
	 *
	 * @param array $estuf
	 * @return html
	 */
	public function montaLegendaLeiAdesaoEstado( Array $estuf, $tipo = null ){
            global $db;?>
            <div id="legendaMapaContainer">
            <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
                <ul>
                    <?php
                        switch( $tipo ){
                            case 'lei':
                                $sqlComLei = "SELECT
                                                count(aseid) AS total
                                              FROM sase.assessoramentoestado
                                              WHERE asestatus = 'A'
                                              AND aseleipne is not null
                                              " . ( ($estuf!=''&&count($estuf) > 0) ? " AND estuf in ( '". ( implode( "','", $estuf ) ) ."' ) ":"") . "";

                                $totalComLei = $db->pegaUm( $sqlComLei );

                                $sqlSemLei = "SELECT
                                                count(aseid) AS total
                                              FROM sase.assessoramentoestado
                                              WHERE asestatus = 'A'
                                              AND aseleipne is null
                                              " . ( ($estuf!=''&&count($estuf) > 0) ? " AND estuf in ( '". ( implode( "','", $estuf ) ) ."' ) ":"") . "";
                                $totalSemLei = $db->pegaUm( $sqlSemLei );
                            break;
                            case 'adesao':
                                $sqlComAdesao = "SELECT
                                                    count(aseid) as total
                                                    FROM sase.assessoramentoestado
                                                    WHERE asestatus = 'A'
                                                    AND aseadesao is not null
                                                    " . (($estuf!=''&&count($estuf)>0)?" AND estuf in ( '". (implode( "','", $estuf )) ."' ) ":"") . " ";
                                $totalComAdesao = $db->pegaUm( $sqlComAdesao );
                                $sqlSemAdesao = " SELECT
                                                    count(aseid) AS total
                                                    FROM sase.assessoramentoestado
                                                    WHERE asestatus = 'A'
                                                    AND aseadesao is null
                                                    ".( ( $estuf != '' && count($estuf) > 0 ) ? " AND estuf in ( '". (implode( "','", $estuf )) ."' ) " : "" )." ";
                                $totalSemAdesao = $db->pegaUm( $sqlSemAdesao );
                            break;
                        }
                            $totalCom = ($tipo == 'lei') ? $totalComLei : $totalComAdesao;
                            $totalSem = ($tipo == 'lei') ? $totalSemLei : $totalSemAdesao;
                            $texto = ($tipo == 'lei') ? 'Lei' : 'Adesão';
                            echo "<li >
                                   <table>
                                      <tr>
                                         <td>
                                            <span style='background:#006600;' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;
                                                <b>" . ( ($totalCom != '') ? $totalCom : '0' ) . "</b>&nbsp;&nbsp;
                                         </td>
                                         <td>
                                            Com {$texto}
                                         </td>
                                      </tr>
                                      <tr><td>&nbsp;</td></tr>
                                      <tr>
                                         <td>
                                            <span style='background:#FFFFFF;' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;
                                                <b>" . ( ($totalSem != '') ? $totalSem : '0' ) . "</b>&nbsp;&nbsp;
                                         </td>
                                         <td>
                                            Sem {$texto}
                                         </td>
                                      </tr>
                                   </table>
                                  </li>";
                        ?>
                </ul>
            </div><?php
        }

	/**
	 * Monta legenda de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situação - macrocategoria
	 *
	 * @author Sávio Resende
	 * @param array $estuf
	 * @return html
	 * @todo otimizar atraves da query
	 */
	public function montaLegendaMacroCategoria(array $estuf, $tipoinfo = Mapas::TIPO_ADEQUACAO)
    {
		global $db;

        switch ($tipoinfo) {
            case Mapas::TIPO_LEIPNE:
                require_once dirname(__FILE__) . '/Mapa/Metadados/DadoMacroCategoriaLeipne.php';
                $classeDados = 'DadoMacroCategoriaLeipne';
                $label = "Lei PNE";
                break;
            case Mapas::TIPO_ADESAO:
                require_once dirname(__FILE__) . '/Mapa/Metadados/DadoMacroCategoriaAdesao.php';
                $classeDados = 'DadoMacroCategoriaAdesao';
                $label = "Adesão";
                break;
            case Mapas::TIPO_ADEQUACAO:
            default:
                require_once dirname(__FILE__) . '/Mapa/Metadados/DadoMacroCategoria.php';
                $classeDados = 'DadoMacroCategoria';
                $label = "Adequação";
        }
        ?>
		<div id="chamadaLegenda" style="display:none"><a onclick="jQuery('#legendaConteudo').css({display:'block'});jQuery('#chamadaLegenda').css({display:'none'})" title="Mostrar Legenda"><img src="../imagens/sase/menu.png" style="width:15px;margin-top:4px;" /></a></div>
		<div id="legendaConteudo" style="display:block;width:150px;" class="topo_bootstrap_off">
			<div id="tituloLegenda">
                <div style="float:left;"><h5>&nbsp;Legenda</h5></div>
				<div style="float:right;"><a onclick="jQuery('#legendaConteudo').css({display:'none'});jQuery('#chamadaLegenda').css({display:'block'})" title="Fechar Legenda"><img style="width:19px;"  src="../imagens/sase/sair.png" /></a></div>
                <br style="clear:both" /><hr style="margin: 2px" />
                <div style="clear:both;height:1px;"><b>Mapa:</b> <?=$label?></div>
				<div style="clear:both;height:1px;">&nbsp;</div>
                <br style="clear:both" /><hr style="margin: 2px" />
			</div>
			<ul>
        <?php

        $legenda = (new $classeDados())->setEstuf($estuf)->dadosDaLegenda();

        foreach ($legenda as $value) {
            $total = (($value['total']!='')?$value['total']:'0');

        echo <<<HTML
<li>
    <table>
        <tr>
            <td>
                <span style="background:{$value['maccor']};" class="elementoCor">&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>{$total}</b>&nbsp;&nbsp;
            </td>
            <td class='dsc'>{$value['macdsc']}</td>
        </tr>
    </table>
</li>
HTML;
        }
        ?>
			</ul>
			<div style="font-size:10px;padding-left:10px;">É possível <b>arrastar</b> e fazer <b>zoom</b>.</div>
		</div>
		<?php
	}

	/**
	 * Monta legenda para o Brasil com a situação - macrocategoria
	 *
	 * @author Sávio Resende
	 * @return html
	 * @todo otimizar atraves da query
	 */
	public function montaLegendaMacroCategoriaPais($tipoinfo = Mapas::TIPO_ADEQUACAO)
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
                switch ($tipoinfo) {
                    case Mapas::TIPO_LEIPNE:
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadoEstadosPaisLeipne.php';
                        $classeDados = 'DadoEstadosPaisLeipne';
                        break;
                    case Mapas::TIPO_ADESAO:
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadoEstadosPaisAdesao.php';
                        $classeDados = 'DadoEstadosPaisAdesao';
                        break;
                    case Mapas::TIPO_ADEQUACAO:
                    default:
                        require_once dirname(__FILE__) . '/Mapa/Metadados/DadoEstadosPais.php';
                        $classeDados = 'DadoEstadosPais';
                }
                $legenda = (new $classeDados())->dadosDaLegenda();
				foreach ($legenda as $key => $value) {
                    $total = (($value['total']!='')?$value['total']:'0');
					echo <<<HTML
<li>
    <table>
        <tr>
            <td>
                <span style="background:{$value['cor']};" class='elementoCor'>&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>{$total}</b>&nbsp;&nbsp;
            </td>
            <td class='dsc'>{$value['descricao']}</td>
        </tr>
    </table>
</li>
HTML;
				}
				?>
			</ul>
		</div>
	<?php }
}