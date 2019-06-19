<?php

namespace Modules\Auditoria\Entities;

use Modules\Auditoria\Entities\Base\AuditoriaBase;

class Auditoria extends AuditoriaBase
{

    public static function getNomeTabelaAuditoriaCorrente()
    {
        return "auditoria.auditoria_" . date('Y_m');
    }

    public static function salvarAuditoria($sql)
    {
        if (env('ENV') !== 'production') {
            return;
        };

        preg_match('/(CREATE\s+TABLE|ALTER\s+TABLE|DROP\s+TABLE|SELECT.*|INSERT\s+INTO|UPDATE|DELETE\s+FROM)\s+([A-Za-z0-1.]+).*/smui', utf8_encode($sql), $matches);

        if (!empty($matches[1])) {
            $audtipoCompleto = strtoupper($matches[1]);
            $audtipo = substr($audtipoCompleto, 0, 1);
            $audtabela = strtoupper($matches[2]);

            if ($audtipo != 'S') {
                \DB::table(static::getNomeTabelaAuditoriaCorrente())->insert([
                    [
                        'usucpf' => $_SESSION['usucpforigem'],
                        'mnuid' => $_SESSION['mnuid'],
                        'audsql' => pg_escape_string($sql),
                        'auddata' => date('Y-m-d H:i:s'),
                        'audtabela' => $audtabela,
                        'audtipo' => $audtipo,
                        'audip' => $_SERVER["REMOTE_ADDR"],
                        'sisid' => $_SESSION['sisid'],
                        'audscript' => pg_escape_string($_SERVER['REQUEST_URI'])
                    ]
                ]);

            }
        }

    }
}